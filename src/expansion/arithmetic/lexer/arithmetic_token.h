#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stddef.h>

enum arithmetic_token_type
{
    TOK_PLUS = 0,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_LPAR,
    TOK_RPAR,
    TOK_POW,
    TOK_SEPAND,
    TOK_PIPE,
    TOK_XOR,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_TILDE,
    TOK_NUMBER,
    TOK_END,
    TOK_UNKNOWN
};

struct arithmetic_token
{
    enum arithmetic_token_type type;
    int value;
    struct arithmetic_token *next;
};

const char *token_str(int type);
// const char *token_str(struct arithmetic_token *token);
bool is_valid_arithmetic_syntax(int type1, int type2);
int str_to_arithmetic_type(char *exp);
int eval_arithmetic_char(char *exp, size_t i);
struct arithmetic_token *new_arithmetic_token(int type);
struct arithmetic_token *new_arithmetic_number_token(int value);

#endif /* ! TOKEN_H */
