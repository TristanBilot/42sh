#ifndef ARITHMETIC_TOKEN_H
#define ARITHMETIC_TOKEN_H

#include <stdbool.h>
#include <stddef.h>

enum arithmetic_token_type
{
    TOK_A_PLUS = 0,
    TOK_A_MINUS,
    TOK_A_MULTIPLY,
    TOK_A_DIVIDE,
    TOK_A_LPAR,
    TOK_A_RPAR,
    TOK_A_POW,
    TOK_A_SEPAND,
    TOK_A_PIPE,
    TOK_A_XOR,
    TOK_A_AND,
    TOK_A_OR,
    TOK_A_NOT,
    TOK_A_TILDE,
    TOK_A_MINUS_EQ,
    TOK_A_PLUS_EQ,
    TOK_A_NUMBER,
    TOK_A_END,
    TOK_A_UNKNOWN
};

struct arithmetic_token
{
    enum arithmetic_token_type type;
    int value;
    struct arithmetic_token *next;
};

const char *token_str(int type);
bool is_valid_arithmetic_syntax(int type1, int type2);
int str_to_arithmetic_type(char *exp);
int eval_arithmetic_char(char *exp, size_t i);
struct arithmetic_token *new_arithmetic_token(int type);
struct arithmetic_token *new_arithmetic_number_token(int value);

#endif /* ! ARITHMETIC_TOKEN_H */
