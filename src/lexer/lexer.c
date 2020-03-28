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
    return res;
}

int lex_full(struct lexer *lexer, const char *c, int j)
{
    char *value;
    if ((value = lex_backslash(c, j)))
    {
        append(lexer, new_token_word(value));
        return 1;
    }
    return 0;
}

int lex_part(struct lexer *lexer, struct buffer *buffer, const char *c, size_t *j)
{
    struct token *token = NULL;
    if ((token = lex_io_number(strdup(c), *j)))
    {
        append(lexer, token);
        return 1;
    }
    if ((token = lex_great_less_and(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        (*j)++;
        append(lexer, token);
        return 1;
    }
    if ((token = lex_great_less(strdup(c), *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type != TOK_LESS && token->type != TOK_GREAT) /* <, > */
        {
            if (token->type == TOK_DLESSDASH) /* <<- */
                (*j) += 2;
            else /* <<, >| ... */
                (*j)++;
        }
        append(lexer, token);
        return 1;
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
    while (splitted[i] != '\0')
    {
        buffer = new_buffer();
        c = splitted[i];
        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                if (lex_full(lexer, c, j))
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
    append(lexer, new_token());
}

struct lexer *new_lexer(const char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(200); /* Don't change it or crash in criterion */
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
        return;
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
