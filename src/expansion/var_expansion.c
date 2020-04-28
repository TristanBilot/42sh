#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "expansion.h"
#include "../var_storage/var_storage.h"
#include "../utils/buffer.h"
#include "../utils/xalloc.h"

void new_program_data_storage(int argc, char *argv[])
{ // va voir gdb ouais je regarde la struct logable est null, comment ça peut marcher alors lol logable ?
    program_data = xmalloc(sizeof(struct program_data_storage));
    program_data->last_cmd_status = xcalloc(MAX_STR_LEN, 1);
    program_data->binary_name = argv[0];
    if (argc <= 1)
    {
        program_data->argv = NULL;
        program_data->argc = 0;
        return;
    }
    char **args = xcalloc(argc - 1, sizeof(char *));
    for (int i = 1; i < argc; i++)
        args[i - 1] = argv[i];
    program_data->argv = args;
    program_data->argc = argc - 1;
}

void free_program_data_storage(void)
{
    if (program_data->argv)
        free(program_data->argv);
    free(program_data->last_cmd_status);
    free(program_data);
}

void update_last_status(int status)
{
    sprintf(program_data->last_cmd_status, "%d", status);
}

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
            if (word[i] == '{')
            {
                i++;
                if (word[i] == '{') // ${{ is not an expansion
                    continue;
                int next_brack_idx = get_next_brack_index(word, i);
                if (next_brack_idx == -1)
                    continue;
                char *param = substr(word, i, next_brack_idx - i);
                if (var_exists(param))
                {
                    char *var = get_value(param);
                    for (size_t j = 0; j < strlen(var); j++)
                        append_buffer(buf, var[i]);
                    i = next_brack_idx;
                }
                else
                    break;
            }
            else
            {
                int type = is_special_char(word[i]);
                char *sub = NULL;
                bool should_continue = false;
                if ((sub = substitute_random(word, &i, &should_continue)) ||
                    (sub = substitute_uid(word, &i, &should_continue)) ||
                    (sub = substitute_oldpwd(word, &i, &should_continue)))
                {
                    if (should_continue)
                    {
                        append_string_to_buffer(buf, sub);
                        free(sub);
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
                        free_buffer(b);
                        break;
                    case PAR_AT:
                        b = substitute_star();
                        append_string_to_buffer(buf, b->buf);
                        free_buffer(b);
                        break;
                    case PAR_HASH:
                        sub = substitute_hash();
                        append_string_to_buffer(buf, sub);
                        free(sub);
                        break;
                    case PAR_QUES:
                        append_string_to_buffer(buf, substitute_ques());
                        break;
                    }
                    continue;
                }
                size_t end = get_next_dollar_index(word, i);
                char *param = substr(word, i, end - i);
                if (var_exists(param))
                {
                    char *var = get_value(param);
                    append_string_to_buffer(buf, var);
                }
                i += strlen(param) - 1;
            }
        }
        else
            append_buffer(buf, word[i]);
    }
    append_buffer(buf, '\0');
    char *exp = buf->buf;
    free_buffer(buf);
    buf = NULL;
    return exp;
}

char *substitute_number(char c)
{
    if (c == '0')
        return program_data->binary_name;
    if (!program_data->argv)
        return "";
    char *sub = program_data->argv[(c - '0') - 1];
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
bool next_param_is_printable(char *word, size_t i, size_t param_len)
{
    return strlen(word) - (i + param_len) == 0 || word[i + param_len] == '$';
}

char *substitute_random(char *word, size_t *i, bool *should_continue)
{
    size_t len = strlen("RANDOM");
    if ((*i + len) > strlen(word))
        return NULL;
    
    if (is(substr(word, *i, len), "RANDOM"))
    {
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%d", get_random_int());
        if (next_param_is_printable(word, *i, len))
            *should_continue = true;
        else
            *should_continue = false;
        *i += (len - 1);
        return sub;
    }
    return NULL;
}

char *substitute_uid(char *word, size_t *i, bool *should_continue)
{
    size_t len = strlen("UID");
    if ((*i + len) > strlen(word))
        return NULL;
    
    if (is(substr(word, *i, len), "UID"))
    {
        unsigned long int uid = (unsigned long int) getuid();
        char *sub = xmalloc(MAX_STR_LEN);
        sprintf(sub, "%ld", uid);
        if (next_param_is_printable(word, *i, len))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1;
        return sub;
    }
    return NULL;
}

char *substitute_oldpwd(char *word, size_t *i, bool *should_continue)
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
        if (next_param_is_printable(word, *i, len))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1;
        return sub;
    }
    return NULL;
}

char *substitute_ifs(char *word, size_t *i, bool *should_continue)
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
        if (next_param_is_printable(word, *i, len))
            *should_continue = true;
        else
            *should_continue = false;
        *i += len - 1;
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

size_t get_next_brack_index(const char *c, size_t j)
{
    if (!c)
        return 0;
    while (++j < strlen(c))
        if (c[j] == '}')
            return j;
    return -1;
}

size_t get_next_dollar_index(const char *c, size_t j)
{
    if (!c)
        return 0;
    while (++j < strlen(c))
        if (c[j] == '$')
            return j;
    return strlen(c);
}
