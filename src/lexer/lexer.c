// #define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"
#include "../utils/buffer.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"
#include "../lexer/lex_evaluation.h"
#include "../utils/string_utils.h"
#include "../utils/index_utils.h"
#include "../utils/bracket_counter.h"
#include "../storage/program_data_storage.h"
#include "../storage/var_storage.h"

bool is_word = false;
bool is_kw_in = false;
bool is_ass_w = false;

char **split(char *str)
{
    if (!str)
        return NULL;
    const char delim[3] = " \t";
    char **res = xcalloc(1, sizeof(char *) * MAX_TOKEN);
    char *splitted = strtok(my_strdup(str), delim);
    int i = 0;
    while (splitted != NULL)
    {

        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    res[i] = NULL;
    free(splitted);
    return res;
}

/*
* return 1: break
* return -1: continue
* return 0: nothing
*/
int lex_parenthesis(struct lexer *lexer, struct buffer *buffer, char *c,
    size_t *j)
{
    int type;
    if (c[*j] == ')')
    {
        if (*j == 0) /* )if */
        {
            append(lexer, new_token_type(TOK_RPAREN));
            flush(buffer);
            return -1;
        }
        char *content = substr(c, 0, *j);
        if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
        {
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(TOK_RPAREN));
            if (c[*j + 1])
            {
                flush(buffer);
                return -1;
            }
            return 1;
        }
    }
    else if (c[*j] == '(')
    {
        if (c[(*j + 1)])
        {
            // a revoir le strlen: fi);
            char *content = substr(c, (*j + 1), strlen(c));
            if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
            {
                append(lexer, new_token_type(TOK_LPAREN));
                append(lexer, new_token_type(type));
                return 1;
            }
        }
    }
    return 0;
}

struct token *lex_assignment_word(char *c, size_t *i)
{
    if (!c || !c[*i])
        return NULL;
    if (!(c[*i] == '='))
        return NULL;
    if (*i == 0)
        return NULL;
    is_ass_w = true;
    size_t prev_sep = get_previous_separator_index(c, *i);
    char *var_name = substr(c, prev_sep, *i - prev_sep);
    struct token *token = new_token_type(TOK_ASS_WORD);
    token->value = var_name;
    return token;
}

int lex_separator(struct lexer *lexer, struct buffer *buffer, char *c,
    size_t *j)
{
    if (is_separator(c[*j]))
    {
        int type;
        char tmp[2];
        tmp[0] = c[*j];
        tmp[1] = '\0';
        int separator = evaluate_token(tmp);
        if ((type = evaluate_keyword(buffer->buf)) != KW_UNKNOWN)
        {
            if (is_word)
                append(lexer, new_token_word(buffer->buf));
            else
                append(lexer, new_token_type(type));
            append(lexer, new_token_type(separator));
            flush(buffer);
            is_word = false;
            return -1;
        }
        else if ((type = evaluate_token(buffer->buf) == TOK_WORD))
        {
            if (var_exists(alias_storage, buffer->buf))
                append(lexer, new_token_word(get_value(alias_storage,
                    buffer->buf)));
        }
        is_word = false;
        is_ass_w = false;
    }
    return 0;
}

// int lex_parameter(struct lexer *lexer, struct buffer *buffer, char *c,
//     size_t *j)
// {
//     if (c[*j] == '$' && c[*j + 1] && c[*j + 1] == '{')
//     {
//         append_word_if_needed(lexer, buffer);
//         size_t close_index = c[*j + 1] == '{' ?
//             get_next_close_curl_index(c, *j) :
//             get_next_close_parent_index(c, *j);
//         char *content = substr(c, *j, close_index - *j);
//         append(lexer, new_token_word(content));
//         *j += strlen(content) - 1;
//         flush(buffer);
//         return -1;
//     }
//     return 0;
// }

/*
* echo "a && b"
* echo $(ls pwd  tree .)
*
* return 0: continue lexing
* return -1: continue for loop
* return 1|2: exit status error
*/
int lex_multi_token(struct lexer *lexer, struct buffer *buffer,
    char **splitted, int *i, size_t *j)
{
    char *c = splitted[*i];
    if (!c)
        return 1;
    bool wait_doub_quote = c[*j] == '"';
    bool wait_simp_quote = c[*j] == '\'';
    bool wait_back_quote = c[*j] == '`';
    bool wait_close_paren = c[*j] == '$' && c[*j + 1] && c[*j + 1] == '(';
    bool wait_close_brack = c[*j] == '$' && c[*j + 1] && c[*j + 1] == '{';
    if (wait_doub_quote || wait_simp_quote || wait_back_quote
        || wait_close_paren || wait_close_brack)
    {
        int stuck_left_paren_count = 0;
        int stuck_right_paren_count = 0;
        int stuck_left_brack_count = 0;
        if (wait_doub_quote || wait_simp_quote)
            append_word_if_needed(lexer, buffer);
         /* only for $(): add max 2 (( at the start of word */
        if (wait_close_paren || wait_close_brack)
        {
            append_buffer(buffer, '$');
            (*j)++;
        }
        while (wait_close_brack && c[*j] && c[*j] == '{')
        {
            stuck_left_brack_count++;
            /* ${{a}} should exit with status 1 */
            if (stuck_left_brack_count >= 2)
                return 1;
            append_buffer(buffer, '{');

            (*j)++;
        }
        while (wait_close_paren && c[*j] && c[*j] == '(')
        {
            if (stuck_left_paren_count < 2)
                append_buffer(buffer, '(');
            stuck_left_paren_count++;
            (*j)++;
        }
        /* only for " and ' => ignore theses caracters */
        if (c[*j] == '"') //
            (*j)++;
        else if (c[*j] == '\'') //
        {
            (*j)++; //
            append_buffer(buffer, '\''); //
        } //
        else if (wait_back_quote)
        {
            (*j)++;
            append_buffer(buffer, '`');
        }
        /* start at ' " $ */
        while (splitted[*i])
        {
            c = splitted[*i];
            for (; *j < strlen(splitted[*i]); (*j)++)
            {
                if ((wait_doub_quote && c[*j] == '"')
                    || (wait_simp_quote && c[*j] == '\'')
                    || (wait_close_paren && c[*j] == ')')
                    || (wait_close_brack && c[*j] == '}')
                    || (wait_back_quote && c[*j] == '`'))
                {
                    /* only for $(): add max 2 )) at the end of word */
                    while (wait_close_paren && c[*j] && c[*j] == ')')
                    {
                        if (stuck_right_paren_count < 2)
                            append_buffer(buffer, ')');
                        stuck_right_paren_count++;
                        (*j)++;
                    }
                    while (wait_close_brack && c[*j] && c[*j] == '}')
                    {
                        append_buffer(buffer, '}');
                        (*j)++;
                    }

                    /* if nb of right parenthesis is > than left ones,
                       should exit with status 2 */
                    if (stuck_left_paren_count < stuck_right_paren_count)
                        return 2;
                    /* TODO: when left parenthesis > right, should wait input*/
                    /* need to decrement j because of while loop */
                    if (wait_close_paren || wait_close_brack)
                        (*j)--;
                    if (wait_back_quote)
                        append_buffer(buffer, '`');
                    /* only append word token if it is the last character of a word
                       if not, don't append: ex => $(ls)word */
                    if (wait_doub_quote || wait_simp_quote ||
                        (*j + 1 >= strlen(c)))
                    {
                        if (wait_simp_quote) //
                            append_buffer(buffer, '\''); //
                        append_buffer(buffer, '\0');
                        append(lexer, new_token_word(buffer->buf));
                        flush(buffer);
                    }
                    return -1;
                }
                append_buffer(buffer, c[*j]);
            }
            if (splitted[*i + 1])
                append_buffer(buffer, ' ');
            *j = 0;
            (*i)++;
        }
    }
    return 0;
}

int lex_part(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    struct token *token = NULL;
    if ((token = lex_io_number(c, j)))
    {
    }
    else if ((token = lex_great_less_and(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        (*j)++;
    }
    else if ((token = lex_great_less(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type != TOK_LESS && token->type != TOK_GREAT) /* <, > */
        {
            if (token->type == TOK_DLESSDASH) /* <<- */
                (*j) += 2;
            else /* <<, >| ... */
                (*j)++;
        }
    }
    else if ((token = lex_comments(c, *j)))
    {
    }
    else if ((token = lex_uni_character(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type == KW_DSEMI) /* ;; */
            (*j)++;
    }
    else if (!is_ass_w && (token = lex_assignment_word(c, j)))
    {
        append(lexer, token);
        flush(buffer);
    }
    append(lexer, token);
    return token ? -1 : 0;
}

int lex_backslash(struct buffer *buffer, char *c, size_t *j)
{
    if (c[*j] != '\\')
        return 0;
    append_buffer(buffer, c[++(*j)]);
    return -1;
}

void substitute_alias(struct lexer *lexer)
{
    if (!lexer->input)
        return;
    struct buffer *buffer = new_buffer();
    struct buffer *fast_buffer = new_buffer();
    size_t j = 0;
    for (size_t i = 0; i < strlen(lexer->input); i++)
    {
        j = i;
        if (i == 0 || !lexer->input[i - 1] || lexer->input[i - 1] == ' '
            || lexer->input[i - 1] == '\t'
            || is_separator(lexer->input[i - 1]))
        {
            while ((j < strlen(lexer->input)) && (lexer->input[j] != ' '
                && !is_separator(lexer->input[j])))
                append_buffer(fast_buffer, lexer->input[j++]);

            if (var_exists(alias_storage, fast_buffer->buf))
            {
                append_string_to_buffer(buffer,
                    get_value(alias_storage, fast_buffer->buf));
                i += strlen(fast_buffer->buf);
            }
        }
        append_buffer(buffer, lexer->input[i]);
        flush(fast_buffer);
    }
    lexer->input = buffer->buf;
}

bool init_lexer(struct lexer *lexer)
{
    substitute_alias(lexer);
    char **splitted = split(lexer->input);
    int i = 0;
    int type;
    int kw;
    while (splitted[i])
    {
        struct buffer *buffer = new_buffer();
        char *c = splitted[i];
        is_ass_w = false;

        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                if ((type = lex_multi_token(lexer, buffer,
                    splitted, &i, &j)) == -1)
                {
                    c = splitted[i];
                    continue;
                }
                else if (type != 0)
                {
                    error("Lexer error: invalid parenthesis");
                    update_last_status(type);
                    return false;
                }
                if ((type = lex_separator(lexer, buffer, c, &j)) == -1)
                    continue;
                if ((type = lex_parenthesis(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                // if ((type = lex_parameter(lexer, buffer, c, &j)) == -1)
                //     continue;
                if ((type = lex_part(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                if ((type = lex_backslash(buffer, c, &j)) != -1)
                    append_buffer(buffer, c[j]);
                if (j == strlen(c) - 1)
                {
                    if (((type = evaluate_keyword(buffer->buf)) != KW_UNKNOWN)
                        && !is_word && !is_ass_w)
                        append(lexer, new_token_type(type));
                    else
                        append(lexer, new_token_word(buffer->buf));
                    is_word = true;
                    flush(buffer);
                }
            }
        }
        /* CASE AND IN ARE EXCEPTIONS: case a in b */
        else if (((kw = evaluate_keyword(c)) != KW_UNKNOWN && is_word
            && (kw != KW_IN || !is_kw_in)))
            append(lexer, new_token_word(c));
        else
        {
            append(lexer, new_token_type(type));
            is_word = false;
            if (type == KW_CASE || type == KW_FOR)
                is_kw_in = true;
            if (type == KW_IN)
                is_kw_in = false;
        }
        i++;
    }
    is_word = is_kw_in = is_ass_w = false;
    append(lexer, new_token_type(TOK_EOF));
    return true;
}

struct lexer *new_lexer(char *str)
{
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
    lexer->token_list->first = NULL;
    lexer->token_list->next = NULL;
    lexer->token_list->last = NULL;
    lexer->input = str;
    bool state = init_lexer(lexer);
    return state ? lexer : NULL;
}

struct token *peek(struct lexer *lexer)
{
    if (!lexer->token_list || !lexer->token_list->next)
        return NULL;
    return lexer->token_list->next;
}

struct token *pop(struct lexer *lexer)
{
    if (!lexer->token_list || !lexer->token_list->next)
        return NULL;
    struct token *next = lexer->token_list->next;
    lexer->token_list->next = next->next;
    return next;
}

void append(struct lexer *lexer, struct token *token)
{
    if (!lexer || !token)
        return;
    if (!lexer->token_list->last)
    {
        lexer->token_list->next = token;
        lexer->token_list->last = token;
        lexer->token_list->first = token;
        lexer->token_list->next->next = NULL;
        return;
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
