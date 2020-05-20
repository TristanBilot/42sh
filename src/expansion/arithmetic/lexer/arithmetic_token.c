#include "arithmetic_token.h"
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const char *token_str(struct arithmetic_token *token)
// {
//     static const char *token_strs[] = { "+", "-", "*", "/", "(", ")", "**", "&", "|", "^", "&&", "||", "!", "~" };
//     static char number_str[15];

//     if (token->type != TOK_A_NUMBER)
//         return token_strs[token->type];
//     sprintf(number_str, "%d", token->value);
//     return number_str;
// }

const char *token_str(int type)
{
    static const char *token_strs[] = { "+", "-", "*", "/", "(", ")", "**",
        "&","|", "^", "&&", "||", "!", "~" };
    if (type == 14)
        return "NUMBER";
    if (type == 15)
        return "TOK_A_END";
    if (type == 16)
        return "TOK_A_UNKNOWN";
    return token_strs[type];
}

bool is_parenhesis(int type)
{
    return type == TOK_A_LPAR || type == TOK_A_RPAR;
}

bool is_plus_or_minus(int type)
{
    return type == TOK_A_PLUS || type == TOK_A_MINUS;
}

bool is_valid_arithmetic_syntax(int type1, int type2)
{
    // int n = eval_arithmetic_char(exp, i);
    // int m = eval_arithmetic_char(exp, i + 1);
    // printf("%s %s\n", token_str(n), token_str(m));
    if (type2 == TOK_A_UNKNOWN)
        return false;
    if (type1 != TOK_A_NUMBER && type2 != TOK_A_NUMBER && type2 != TOK_A_END)
    {
        if ((type1 == TOK_A_LPAR && type2 == TOK_A_LPAR)
            ||  (type2 == TOK_A_RPAR && type1 == TOK_A_RPAR))
            return true;
        if ((type1 == TOK_A_LPAR && type2 == TOK_A_RPAR)
            || (type2 == TOK_A_LPAR && type1 == TOK_A_RPAR))
            return false;
        if ((is_parenhesis(type1) && !is_parenhesis(type2))
            || (is_parenhesis(type2) && !is_parenhesis(type1)))
            return true;
        if (is_plus_or_minus(type1) || is_plus_or_minus(type2))
            return true;
        if ((is_plus_or_minus(type1) && (type2 == TOK_A_MULTIPLY
            || type2 == TOK_A_DIVIDE)) || (is_plus_or_minus(type2)
            &&  (type1 == TOK_A_MULTIPLY || type1 == TOK_A_DIVIDE)))
            return true;
        return false;
    }
    return true;
}

int str_to_arithmetic_type(char *exp)
{
    if (is(exp, "+"))
        return TOK_A_PLUS;
    else if (is(exp, "-"))
        return TOK_A_MINUS;
    else if (is(exp, "*"))
        return TOK_A_MULTIPLY;
    else if (is(exp, "/"))
        return TOK_A_DIVIDE;
    else if (is(exp, "("))
        return TOK_A_LPAR;
    else if (is(exp, ")"))
        return TOK_A_RPAR;
    else if (is(exp, "**"))
        return TOK_A_POW;
    else if (is(exp, "&"))
        return TOK_A_SEPAND;
    else if (is(exp, "|"))
        return TOK_A_PIPE;
    else if (is(exp, "^"))
        return TOK_A_XOR;
    else if (is(exp, "&&"))
        return TOK_A_AND;
    else if (is(exp, "||"))
        return TOK_A_OR;
    else if (is(exp, "!"))
        return TOK_A_NOT;
    else if (is(exp, "~"))
        return TOK_A_TILDE;
    else if (expr_is_number(exp))
        return TOK_A_NUMBER;
    return TOK_A_UNKNOWN;
}

int eval_arithmetic_char(char *exp, size_t i)
{
    if (i >= strlen(exp))
        return TOK_A_UNKNOWN;
    char c  = exp[i];
    if (c == '+')
        return TOK_A_PLUS;
    else if (c == '-')
        return TOK_A_MINUS;
    else if (c == '*' && strlen(exp) > i && exp[i+1] == '*')
        return TOK_A_POW;
    else if (c == '&' && strlen(exp) > i && exp[i+1] == '&')
        return TOK_A_AND;
    else if (c == '|' && strlen(exp) > i && exp[i+1] == '|')
        return TOK_A_OR;
    else if (c == '/')
        return TOK_A_DIVIDE;
    else if (c == '(')
        return TOK_A_LPAR;
    else if (c == ')')
        return TOK_A_RPAR;
    else if (c == '*')
        return TOK_A_MULTIPLY;
    else if (c == '&')
        return TOK_A_SEPAND;
    else if (c == '|')
        return TOK_A_PIPE;
    else if (c == '^')
        return TOK_A_XOR;
    else if (c == '!')
        return TOK_A_NOT;
    else if (c == '~')
        return TOK_A_TILDE;
    else if (is_number(c))
        return TOK_A_NUMBER;
    return TOK_A_UNKNOWN;
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
    res->type = TOK_A_NUMBER;
    res->value = value;
    return res;
}
