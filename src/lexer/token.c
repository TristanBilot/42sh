#include "../lexer/token.h"
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"

#include <stdio.h>
#include <stdlib.h>

struct token *new_token(void)
{
    struct token *new = xmalloc(sizeof(struct token));
    new->type = TOK_EOF;
    new->value = "";
    new->next = NULL;
    return new;
}

struct token *new_token_type(int type)
{
    struct token *new = new_token();
    new->type = type;
    new->value = "";
    return new;
}

struct token *new_token_io_number(char number)
{
    struct token *new = new_token_type(TOK_IONUMBER);
    char *tmp = xmalloc(2 * sizeof(char));
    tmp[0] = number;
    tmp[1] = '\0';
    new->value = tmp;
    return new;
}

struct token *new_token_word(char *value)
{
    struct token *new = new_token();
    new->type = TOK_WORD;
    new->value = xmalloc(MAX_TOKEN);  /* FREE */
    strcpy(new->value, value);
    return new;
}

struct token *new_token_error(char *err)
{
    error(err);
    return new_token_type(TOK_ERROR);
}

void free_token(struct token *token)
{
    free(token);
}

int is_type(struct token *token, unsigned int type)
{
    return token->type == type;
}
