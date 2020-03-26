#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/xalloc.h"
#include "lexer/lexer.h"
#include "token.h"

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

int evaluate_number(char *str, size_t *index)
{
    int res = 0;
    while (index && *index < strlen(str) &&
        str[*index] >= '0' && str[*index] <= '9')
    {
        res *= 10;
        res += (str[(*index)++] - '0');
    }
    (*index)--;
    return res;
}

// struct token *lex_full_token(int type, char *token)
// {
//     struct token *new = new_token();
//     if (type == TOKEN_NUMBER)
//         new->value = atoi(token);
//     new->type = type;
//     return new;
// }

// struct token *lex_token(int type, char *token, size_t index)
// {
//     struct token *new = new_token();
//     new->type = type;
//     return new;
// }

void flush(char *buffer, int size)
{
    for (int i = 0; i < size; i++)
        buffer[i] = '\0';
}

void lexer_init(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    char *c;
    int i = 0;
    int type;
    char buffer[MAX_STR_LEN];
    while (splitted[i] != '\0')
    {
        c = splitted[i];
        if ((type = evaluate_token(c)) < 0)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                char buf[2];
                buf[0] = c[j];
                buf[1] = '\0';
                strcat(buffer, buf);
                if ((type = evaluate_token(buffer)) >= 0 && 
                    c[j+1] && (c[j+1] == ' ' || c[j+1] == '\n'))
                {
                    append(lexer, new_token_type(type));
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
    lexer_init(lexer);
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
