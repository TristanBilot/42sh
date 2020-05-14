#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utils/xalloc.h"
#include "lexer.h"
#include "../lexer/lexer.h"
#include "../utils/string_utils.h"
#include "../utils/buffer.h"

int evaluate_number(char *str, size_t *index)
{
    int res = 0;
    while (index && *index < strlen(str) &&
        str[*index] >= '0' && str[*index] <= '9')
    {
        res *= 10;
        res += (str[(*index)++] - '0');
    }
    return res;
}

struct lexer *lexer_alloc(const char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->input = str;
    lexer->index = 0;
    lexer->length = 0;
    char *c;
    struct token *token = NULL;
    struct buffer *buf = new_buffer();

    for (size_t i = 0; i < strlen(str); i++)
    {
        // append_buffer(buf, )
    }

    char **splitted = split(str);
    int i = 0;
    while (splitted[i] != '\0')
    {
        c = splitted[i];
        token = token_alloc();
        int type;

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
                    struct token *token2 = token_alloc();
                    token2->type = type;
                    if (type == TOKEN_NUMBER)
                    {
                        token2->value = evaluate_number(c, &j);
                        j--;
                    }
                    lexer->token_list[lexer->index++] = token2;
                }
            }
        }
        else
        {
            if (type == TOKEN_NUMBER)
                token->value = atoi(c);
            token->type = type;
            lexer->token_list[lexer->index++] = token;
        }
        i++;
    }
    lexer->token_list[lexer->index++] = token_alloc();
    lexer->length = lexer->index;
    lexer->index = 0;
    return lexer;
}

struct arithmetic_lexer *new_arithmetic_lexer(char *str)
{
    struct arithmetic_lexer *lexer = xmalloc(sizeof(struct arithmetic_lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
    lexer->token_list->first = NULL;
    lexer->token_list->next = NULL;
    lexer->token_list->last = NULL;
    lexer->input = str;
    bool state = init_lexer(lexer);
    return state ? lexer : NULL;
}


struct arithmetic_token *peek(struct arithmetic_lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    return lexer->token_list->next;
}

struct arithmetic_token *pop(struct arithmetic_lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    struct token *next = lexer->token_list->next;
    lexer->token_list->next = next->next;
    return next;
}

void append(struct arithmetic_lexer *lexer, struct arithmetic_token *token)
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
