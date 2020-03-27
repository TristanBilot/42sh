#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/xalloc.h"
#include "utils/string.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include "lexer/lex_evaluation.h"

#define MAX_STR_LEN 256

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
    return res;
}

void init_lexer(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    char *c;
    int i = 0;
    int type;
    struct buffer *buffer = NULL;
    while (splitted[i] != '\0')
    {
        buffer = calloc(sizeof(struct buffer), 1);
        buffer->buf = calloc(256,1);
        c = splitted[i];
        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                if (lex_backslash(c, j))
                {
                    append(lexer, new_token_word(lex_backslash(c, j)));
                    flush(buffer, MAX_STR_LEN);
                    
                    break;
                }
                
                buffer->buf[buffer->index++] = c[j];
                if ((type = evaluate_token(buffer->buf)) != TOK_WORD && 
                    c[j+1] && (c[j+1] == ' ' || c[j+1] == '\n'))
                {
                    append(lexer, new_token_type(type));
                    flush(buffer, MAX_STR_LEN);
                }
                if (j == strlen(c) - 1)
                {
                    append(lexer, new_token_word(c));
                    flush(buffer, MAX_STR_LEN);
                }
            }
        }
        else
            append(lexer, new_token_type(type));
        i++;
    }
    append(lexer, new_token());
}

struct lexer *new_lexer(const char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
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
        free(tmp);
        index = index->next;
    }
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
        !lexer->token_list->next ||
        !lexer->token_list->next->next)
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
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
