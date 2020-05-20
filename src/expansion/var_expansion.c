#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#include "expansion.h"
#include "../storage/var_storage.h"
#include "../utils/buffer.h"
#include "../utils/xalloc.h"
#include "../utils/index_utils.h"
#include "../storage/program_data_storage.h"

char *perform_var_expansion(char *word)
{
    struct buffer *buf = new_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (word[i] == '$')
        {
            i++;
            if (word[i] == '\0' || word[i] == '\n')
            {
                append_buffer(buf, '$');
                break;
            }
            if (word[i] == '(')
            {
                append_buffer(buf, '$');
                append_buffer(buf, '(');
                continue;
            }
            int is_brack = 0;
            if (word[i] == '{')
            {
                is_brack = 1;
                i++;
            }
            int type = is_special_char(word[i]);
            char *sub = NULL;
            bool should_continue = false;
            if ((sub = substitute_random(word, &i, &should_continue, is_brack))
                || (sub = substitute_uid(word, &i, &should_continue, is_brack))
                || (sub = substitute_pid(word, &i, &should_continue, is_brack))
                || (sub = substitute_oldpwd(word, &i,
                    &should_continue, is_brack))
                || (sub = substitute_ifs(word, &i,
                    &should_continue, is_brack)))
            {
                if (should_continue)
                {
                    append_string_to_buffer(buf, sub);
                    continue;
                }
                else
                    break;
            }
            if (type != PAR_UNKNOWN)
            {
                struct buffer *b = NULL;
                switch (type)
                {
                case PAR_NUMBER:
                    append_string_to_buffer(buf, substitute_number(word[i]));
                    break;
                case PAR_STAR:
                    b = substitute_star();
                    append_string_to_buffer(buf, b->buf);
                    break;
                case PAR_AT:
                    b = substitute_star();
                    append_string_to_buffer(buf, b->buf);
                    break;
                case PAR_HASH:
                    sub = substitute_hash();
                    append_string_to_buffer(buf, sub);
                    break;
                case PAR_QUES:
                    append_string_to_buffer(buf, substitute_ques());
                    break;
                }
                if (is_brack)
                    i++;
                continue;
            }
            size_t next_close_paren = get_next_index(word, ')', i);
            size_t next_back_quote = get_next_index(word, '`', i);
            size_t next_close_dollar = get_next_index(word, '$', i);
            size_t next_separator = get_next_separator_index(word, i);
            size_t next_close_curl = get_next_index(word, '}', i);
            size_t end = 0;
            if (is_brack)
                end = next_close_curl;
            else
            {
                /* find the nearest possible delimitor stuck to the variable */
                end = next_close_paren < next_close_dollar ?
                    next_close_paren : next_close_dollar;
                end = end < next_separator ? end : next_separator;
                end = end < next_back_quote ? end : next_back_quote;
            }
            char *param = substr(word, i, end - i);
            char *env = xcalloc(1, 256);
            if ((env = getenv(param)))
                append_string_to_buffer(buf, env);
            else if (var_exists(var_storage, param))
            {
                char *var = get_value(var_storage, param);
                append_string_to_buffer(buf, var);
            }
            i += strlen(param) - 1 + is_brack;
        }
        else
            append_buffer(buf, word[i]);
    }
    append_buffer(buf, '\0');
    return buf->buf;
}

char *substitute_number(char c)
{
    if (c == '0')
        return program_data->binary_name;
    if (!program_data->argv)
        return "";
    int index = (c - '0') - 1;
    char *sub = NULL;
    if (index < program_data->argc)
        sub = program_data->argv[index];
    return sub ? sub : "";
}

struct buffer *substitute_star(void)
{
    struct buffer *buf = new_buffer();
    for (int i = 0; i < program_data->argc; i++)
    {
        append_string_to_buffer(buf, program_data->argv[i]);
        if (i != program_data->argc - 1)
            append_buffer(buf, ' ');
    }
    append_buffer(buf, '\0');
    return buf;
}

struct buffer *substitute_at(void)
{
    return substitute_star();
}

char *substitute_hash(void)
{
    char *sub = xmalloc(MAX_STR_LEN);
    sprintf(sub, "%d", program_data->argc);
    return sub;
}

char *substitute_ques(void)
{
    return program_data->last_cmd_status;
}

/*
* The successor string of a parameter should be another parameter or nothing.
* $RANDOM$UID$1 is right
* $RANDOMstring is wrong
*/
bool next_param_is_printable(char *word, size_t i, size_t param_len,
    bool is_brack)
{
    if (is_brack && word[i +param_len] != '}')
        return false;
    return strlen(word) - (i + param_len + is_brack) == 0
        || word[i + param_len + is_brack] == '$';
}

char *substitute_random(char *word, size_t *i, bool *should_continue,
    int is_brack)
{
    size_t len = strlen("RANDOM");
    if ((*i + len) > strlen(word))
        return NULL;
    if (is(substr(word, *i, len), "RANDOM"))
    {
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%d", get_random_int());
        if (next_param_is_printable(word, *i, len, is_brack))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1 + is_brack;
        return sub;
    }
    return NULL;
}

char *substitute_uid(char *word, size_t *i, bool *should_continue,
    int is_brack)
{
    size_t len = strlen("UID");
    if ((*i + len) > strlen(word))
        return NULL;
    if (is(substr(word, *i, len), "UID"))
    {
        unsigned long int uid = (unsigned long int) getuid();
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%ld", uid);
        if (next_param_is_printable(word, *i, len, is_brack))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1 + is_brack;
        return sub;
    }
    return NULL;
}

char *substitute_pid(char *word, size_t *i, bool *should_continue,
    int is_brack)
{
    size_t len = strlen("$");
    if ((*i + len) > strlen(word))
        return NULL;
    if (is(substr(word, *i, len), "$"))
    {
        unsigned long int uid = (unsigned long int) getpid();
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%ld", uid);
        if (next_param_is_printable(word, *i, len, is_brack))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1 + is_brack;
        return sub;
    }
    return NULL;
}

char *substitute_oldpwd(char *word, size_t *i, bool *should_continue,
    int is_brack)
{
    char *old_pwd = getenv("OLDPWD");
    if (!old_pwd)
        return NULL;

    size_t len = strlen("OLDPWD");
    if ((*i + len) > strlen(word))
        return NULL;
    if (is(substr(word, *i, len), "OLDPWD"))
    {
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%s", old_pwd);
        if (next_param_is_printable(word, *i, len, is_brack))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1 + is_brack;
        return sub;
    }
    return NULL;
}

char *substitute_ifs(char *word, size_t *i, bool *should_continue,
    int is_brack)
{
    char *ifs = getenv("IFS");
    if (!ifs)
        return NULL;

    size_t len = strlen("IFS");
    if ((*i + len) > strlen(word))
        return NULL;
    if (is(substr(word, *i, len), "IFS"))
    {
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%s", ifs);
        if (next_param_is_printable(word, *i, len, is_brack))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1 + is_brack;
        return sub;
    }
    return NULL;
}

enum param_type is_special_char(char c)
{
    if (is_number(c))
        return PAR_NUMBER;
    switch (c)
    {
    case '*':
        return PAR_STAR;
    case '@':
        return PAR_AT;
    case '?':
        return PAR_QUES;
    case '#':
        return PAR_HASH;
    default:
        return PAR_UNKNOWN;
    }
}

int get_random_int(void)
{
    return rand() % (32767 + 1);
}
