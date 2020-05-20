#ifndef ARITHMETIC_LEXER_H
#define ARITHMETIC_LEXER_H

#include "arithmetic_token.h"

struct arithmetic_lexer
{
    char *input;
    struct arithmetic_token_list *token_list;
};

struct arithmetic_token_list
{
    struct arithmetic_token *last;
    struct arithmetic_token *first;
    struct arithmetic_token *next;
};

bool init_arithmetic_lexer(struct arithmetic_lexer *lexer);
struct arithmetic_lexer *new_arithmetic_lexer(char *str);
struct arithmetic_token *peek_arithmetic(struct arithmetic_lexer *lexer);
struct arithmetic_token *pop_arithmetic(struct arithmetic_lexer *lexer);
void append_arithmetic(struct arithmetic_lexer *lexer, struct arithmetic_token *token);

#endif /* ! ARITHMETIC_LEXER_H */
