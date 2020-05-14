#include "token.h"
#include "../utils/xalloc.h"

#include <stdio.h>
#include <stdlib.h>

const char *token_str(struct arithmetic_token *token)
{
    static const char *token_strs[] = { "+", "-", "*", "/", "(", ")", "**", "&", "|", "^", "&&", "||", "!", "~" };
    static char number_str[15];

    if (token->type != TOKEN_NUMBER)
        return token_strs[token->type];
        
    sprintf(number_str, "%d", token->value);
    return number_str;
}

bool expr_is_number(char *expr)
{
    for (size_t i = 0; i < strlen(expr); i++)
        if (!is_number(expr[i]))
            return false;
    return true;
}

int str_to_arithmetic_type(char *exp)
{
    if (is(exp, "+"))
        return TOK_PLUS;
    else if (is(exp, "-"))
        return TOK_MINUS;
    else if (is(exp, "*"))
        return TOK_MULTIPLY;
    else if (is(exp, "/"))
        return TOK_DIVIDE;
    else if (is(exp, "("))
        return TOK_LPAR;
    else if (is(exp, ")"))
        return TOK_RPAR;
    else if (is(exp, "**"))
        return TOK_POW;
    else if (is(exp, "&"))
        return TOK_SEPAND;
    else if (is(exp, "|"))
        return TOK_PIPE;
    else if (is(exp, "^"))
        return TOK_XOR;
    else if (is(exp, "&&"))
        return TOK_AND;
    else if (is(exp, "||"))
        return TOK_OR;
    else if (is(exp, "!"))
        return TOK_NOT;
    else if (is(exp, "~"))
        return TOK_TILDE;
    else if (expr_is_number(exp))
        return TOK_NUMBER;
    return TOK_UNKNOWN;
}

bool is_arithmetic_symbol(char c)
{
    const char symbols[] = { '+', '-', '*', '/', '(', ')', '&', '|', '^', '!', '~'};
    while (*symbols)
        if (c == (*symbols)++)
            return true;
    return false;
}

struct arithmetic_token *new_arithmetic_token(int type)
{
    struct arithmetic_token *res = xcalloc(1, sizeof(*res));
    res->type = type;
    res->value = 0;
    return res;
}

struct arithmetic_token *new_arithmetic_number_token(int value)
{
    struct arithmetic_token *res = xcalloc(1, sizeof(*res));
    res->type = TOK_NUMBER;
    res->value = value;
    return res;
}
