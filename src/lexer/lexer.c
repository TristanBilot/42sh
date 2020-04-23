#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/xalloc.h"
#include "utils/string.h"
#include "utils/buffer.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include "lexer/lex_evaluation.h"

char **split(const char *str)
{
    char delim[] = " ";
    char **res = calloc(1, sizeof(char *) * MAX_TOKEN);
    char *splitted = strtok(strdup(str), delim);
    int i = 0;
    while (splitted != NULL)
    {
        res[i] = xmalloc(sizeof(char *));
        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    res[i] = NULL;
    free(splitted);
    return res;
}

int lex_full(struct lexer *lexer, const char *c, size_t j)
{
    char *value = NULL;
    struct token *token = NULL;
    if ((value = lex_backslash(c, j)))
        append(lexer, new_token_word(value));
    else if ((token = lex_assignment_word(strdup(c), &j))) {
        append(lexer, token);
        if ((token = lex_assignment_value(strdup(c), ++j)))
            append(lexer, token);
    }
    return (token || value) ? 1 : 0;
}

int lex_part(struct lexer *lexer, struct buffer *buffer, const char *c, size_t *j)
{
    struct token *token = NULL;
    char *copy = strdup(c);
    if ((token = lex_io_number(copy, *j))) { }
    else if ((token = lex_great_less_and(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        (*j)++;
    }
    else if ((token = lex_great_less(copy, *j)))
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
    else if ((token = lex_comments(copy, *j))) { }
    else if ((token = lex_uni_character(copy, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type == KW_DSEMI) /* ;; */
            (*j)++;
    }
    append(lexer, token);
    return token ? 1 : 0;
}

/*
* return 1: break
* return -1: continue
* return 0: nothing
*/
int lex_parenthesis(struct lexer *lexer, struct buffer *buffer, const char *c, size_t *j)
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
        char *content = substr(strdup(c), 0, *j);
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
            char *content = substr(strdup(c), (*j + 1), strlen(c));
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

size_t get_previous_newline_index(const char *c, size_t j)
{
    if (!c)
        return 0;
    while (j > 1 && c[--j])
        if (c[j-1] == '\n')
            return j;
    return 0;
}

/*
* if\n should be IF NEWLINE
*/
int lex_stuck_newline(struct lexer *lexer, struct buffer *buffer, const char *c, size_t *j)
{
    int type;
    if (c[*j] == '\n')
    {
        if (*j == 0 && c[*j+1])
        {
            char *content = substr(strdup(c), (*j + 1), strlen(c));
            if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
            {
                append(lexer, new_token_type(TOK_NEWLINE));
                append(lexer, new_token_type(type));
                return 1;
            }
        }
        size_t starting_index = get_previous_newline_index(c, *j);
        char *content = substr(strdup(c), starting_index, *j - starting_index);
        if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
        {
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(TOK_NEWLINE));
            if (c[*j + 1])
            {
                flush(buffer);
                return -1;
            }
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    char *c;
    int i = 0;
    int type;
    struct buffer *buffer = NULL;
    while (splitted[i])
    {
        buffer = new_buffer();
        c = splitted[i];

        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                if (lex_full(lexer, c, j))
                    break;
                if ((type = lex_parenthesis(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                if ((type = lex_stuck_newline(lexer, buffer, c, &j)) == -1)
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
        free_buffer(buffer);
    }
    append(lexer, new_token_type(TOK_EOF));
    free(splitted);
}

struct lexer *new_lexer(const char *str) {
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
    while (index && index->next)
    {
        tmp = index;
        index = index->next;
        //free(tmp);
    }
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
