#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

enum arithmetic_token_type
{
    TOK_PLUS = 0,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_LPAR,
    TOK_RPAR,
    TOK_NUMBER,
    TOK_POW,
    TOK_SEPAND,
    TOK_PIPE,
    TOK_XOR,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_TILDE,
    TOK_UNKNOWN
};

struct arithmetic_token
{
    enum arithmetic_token_type type;
    int value;
    struct arithmetic_token *next;
};

const char *token_str(struct arithmetic_token *token);
int str_to_arithmetic_type(char *exp);
struct arithmetic_token *new_arithmetic_token(int type);
struct arithmetic_token *new_arithmetic_number_token(int value);

#endif /* ! TOKEN_H */
