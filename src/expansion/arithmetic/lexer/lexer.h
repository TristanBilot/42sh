#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct arithmetic_lexer {
    char *input;
    struct arithmetic_token_list *token_list;
};

struct arithmetic_token_list
{
    struct arithmetic_token *last;
    struct arithmetic_token *first;
    struct arithmetic_token *next;
};

struct arithmetic_lexer *lexer_alloc(const char *str);
struct token *lexer_peek(struct arithmetic_lexer *lexer);
struct token *lexer_pop(struct arithmetic_lexer *lexer);

#endif /* ! LEXER_H */
