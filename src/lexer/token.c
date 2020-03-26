#include "lexer/token.h"
#include "utils/xalloc.h"

#include <stdio.h>
#include <stdlib.h>

struct token *new_token(void)
{
    struct token *new = xmalloc(sizeof(*new));
    new->type = TOK_EOF;
    new->next = NULL;
    return new;
}

struct token *new_token_type(int type)
{
    struct token *new = new_token();
    new->type = type;
    return new;
}

struct token *new_token_word(const char *value)
{
    struct token *new = new_token();
    new->type = TOK_WORD;
    new->value = value;
    return new;
}

void free_token(struct token *token)
{
    free(token);
}
