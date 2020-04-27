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
    char **res = calloc(1, sizeof(char *) * MAX_TOKEN);
    // char *copy = my_strdup(str);
    // printf("str %s$\n", strtok_r("aa aa", " "));
    //char test[7] = "aa aa ";
    
    char *splitted = strtok(str, delim);  //char *strtok(char *s1, const char *s2);
    int i = 0;
    while (splitted != NULL)
    {
        // res[i] = xmalloc(sizeof(char *));
        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    
    res[i] = NULL;
    free(splitted);
    // free(copy);
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
            free(content);
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(TOK_RPAREN));
            if (c[*j + 1])
            {
                flush(buffer);
                return -1;
            }
            return 1;
        }
        free(content);
    }
    else if (c[*j] == '(')
    {
        if (c[(*j + 1)])
        {
            char *content = substr(c, (*j + 1), strlen(c));
            if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
            {
                free(content);
                append(lexer, new_token_type(TOK_LPAREN));
                append(lexer, new_token_type(type));
                return 1;
            }
            free(content);
        }
    }
    return 0;
}

size_t get_previous_newline_index(char *c, size_t j)
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
int lex_stuck_newline(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    int type;
    if (c[*j] == '\n')
    {
        if (*j == 0 && c[*j+1])
        {
            char *content = substr(c, (*j + 1), strlen(c));
            if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
            {
                free(content);
                append(lexer, new_token_type(TOK_NEWLINE));
                append(lexer, new_token_type(type));
                return 1;
            }
            free(content);
        }
        size_t starting_index = get_previous_newline_index(c, *j);
        char *content = substr(c, starting_index, *j - starting_index);
        if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
        {
            free(content);
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(TOK_NEWLINE));
            if (c[*j + 1])
            {
                flush(buffer);
                return -1;
            }
            return 1;
        }
        free(content);
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
                // if (lex_full(lexer, c, j))
                //     break;
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
