#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/xalloc.h"
#include "lexer/lexer.h"
#include "lexer/lexer_struct.h"

int is(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

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

enum token_type evaluate(char *c)
{
    if (is(c, "+"))
        return TOKEN_PLUS;
    else if (is(c, "-"))
        return TOKEN_MINUS;
    else if (is(c, "*"))
        return TOKEN_MULTIPLY;
    else if (is(c, "/"))
        return TOKEN_DIVIDE;
    else if (is(c, "("))
        return TOKEN_LEFT_PARENTHESIS;
    else if (is(c, ")"))
        return TOKEN_RIGHT_PARENTHESIS;
    else if (c[0] >= '0' && c[0] <= '9')
        return TOKEN_NUMBER;
    return -1;
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

void lexer_append(struct lexer *lexer, struct token *token)
{
    if (lexer->index > lexer->length)
        return;
    lexer->length++;
    lexer->token_list[lexer->index++] = token;
}

struct token *lex_full_token(int type, char *token)
{
    struct token *new = token_alloc();
    if (type == TOKEN_NUMBER)
        new->value = atoi(token);
    new->type = type;
    return new;
}

struct token *lex_part_token(int type, char *token, size_t index)
{
    struct token *new = token_alloc();
    if (type == TOKEN_NUMBER)
        new->value = evaluate_number(token, &index);
    new->type = type;
    return new;
}

void lexer_init(struct lexer *lexer)
{
    struct token *token = NULL;
    char **splitted = split(lexer->input);
    char *c;
    int i = 0;
    int type;
    while (splitted[i] != '\0')
    {
        c = splitted[i];
        token = token_alloc();
        if ((type = evaluate(c)) < 0 || token->type == TOKEN_NUMBER)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                char buf[2];
                buf[0] = c[j];
                buf[1] = '\0';
                type = evaluate(buf);
                if (type >= 0)
                {
                    token = lex_part_token(type, c, j);
                    lexer_append(lexer, token);
                }
            }
        }
        else
        {
            token = lex_full_token(type, c);
            lexer_append(lexer, token);
        }
        i++;
    }
    lexer_append(lexer, token_alloc());
    lexer->index = 0;
}

struct lexer *lexer_alloc(const char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->input = str;
    lexer->index = 0;
    lexer->length = 0;
    lexer_init(lexer);
    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    if (!lexer)
        return;
    free(lexer);
}

struct token *lexer_peek(struct lexer *lexer)
{
    if (!lexer || lexer->index >= lexer->length)
        return NULL;
    return lexer->token_list[lexer->index];
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *last = lexer_peek(lexer);
    if (!last)
        return NULL;
    lexer->index++;
    return last;
}
