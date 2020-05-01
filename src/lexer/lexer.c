#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"
#include "../utils/buffer.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"
#include "../lexer/lex_evaluation.h"
#include "../utils/string_utils.h"

char **split(char *str)
{
    if (!str)
        return NULL;
    const char delim[2] = " ";
    char **res = xcalloc(1, sizeof(char *) * MAX_TOKEN);
    char *splitted = strtok(my_strdup(str), delim);
    int i = 0;
    while (splitted != NULL)
    {
        // res[i] = xmalloc(sizeof(char *) * MAX_TOKEN);
        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    res[i] = NULL;
    free(splitted);
    return res;
}

int lex_full(struct lexer *lexer, char *c, size_t j)
{
    char *value = NULL;
    struct token *token = NULL;
    if ((value = lex_backslash(c, j)))
        append(lexer, new_token_word(value));
    return (token || value) ? 1 : 0;
}

int lex_part(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    struct token *token = NULL;
    if ((token = lex_io_number(c, j))) { }
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
    else if ((token = lex_comments(c, *j))) { }
    else if ((token = lex_uni_character(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type == KW_DSEMI) /* ;; */
            (*j)++;
    }
    else if ((token = lex_assignment_word(c, j))) {
        append(lexer, token);
        flush(buffer);
        if ((token = lex_assignment_value(c, j)))
            flush(buffer);
    }
    append(lexer, token);
    return token ? 1 : 0;
}

/*
* return 1: break
* return -1: continue
* return 0: nothing
*/
int lex_parenthesis(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
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

int is_separator(char c)
{
    return c == '\n' || c == ';' || c == '&';
}

int lex_separator(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
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
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(separator));
            flush(buffer);
            return -1;
        }
    }
    return 0;
}

void init_lexer(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    int i = 0;
    int type;
    while (splitted[i])
    {
        struct buffer *buffer = new_buffer();
        char *c = splitted[i];

        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                // if (lex_full(lexer, c, j))
                //     break;
                if ((type = lex_parenthesis(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                if ((type = lex_separator(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                if (lex_part(lexer, buffer, c, &j))
                    continue;

                append_buffer(buffer, c[j]);
                if (j == strlen(c) - 1)
                {
                    append(lexer, new_token_word(buffer->buf));
                    flush(buffer);
                }
            }
        }
        else
            append(lexer, new_token_type(type));
        i++;
        // free_buffer(buffer);
    }
    append(lexer, new_token_type(TOK_EOF));
}

struct lexer *new_lexer(char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
    lexer->token_list->first = NULL;
    lexer->token_list->next = NULL;
    lexer->token_list->last = NULL;
    lexer->input = str;
    init_lexer(lexer);
    return lexer;
}

void free_lexer(struct lexer *lexer)
{
    if (!lexer)
        return;
    struct token *index = lexer->token_list->first;
    struct token *tmp = NULL;
    while (index)
    {
        tmp = index;
        index = index->next;
        if (tmp->type == TOK_IONUMBER || tmp->type == TOK_WORD || tmp->type == TOK_ASS_WORD)
            free(tmp->value);
        free(tmp);
    }
    if (index)
        free(index);
    free(lexer->token_list);
    free(lexer);
}

struct token *peek(struct lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    return lexer->token_list->next;
}

struct token *pop(struct lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
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
