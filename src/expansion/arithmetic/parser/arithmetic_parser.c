#include "../lexer/arithmetic_lexer.h"
#include "../ast/arithmetic_ast.h"
#include "arithmetic_parser.h"
#include "../../../utils/xalloc.h"
#include <stdio.h>

bool token_is_sop(struct arithmetic_token *token)
{
    return token->type == TOK_A_MULTIPLY || token->type == TOK_A_DIVIDE;
}

bool token_is_eop(struct arithmetic_token *token)
{
    return token->type == TOK_A_PLUS || token->type == TOK_A_MINUS;
}

// input:      log_or EOF
//         |   EOF

// log_or:     log_and ( '||' log_and )

// log_and:    bit_or ( '&&' bit_or )

// bit_or:     bit_xor ( '|' bit_xor )

// bit_xor:    bit_and ( '^' bit_and )

// bit_and:    exp ( '&' exp )

// exp:        sexp ('+' sexp | '-' sexp)*

// sexp:       exponent ('*' exponent | '/' exponent)*

// exponent:   log_not ( '**' log_not)

// log_not:    texp ( '!' texp | '~' texp )

// texp:       NUMBER
//         |   '-' NUMBER
//         |   '(' exp ')'



/* /!\ forward function declaration /!\
**
** This solves a cyclique dependency between the 3 parsing functions
**
**      exp ----> sexp ----> texp +
**       ^                        |
**       + ---------------------- +
**
** this means at least one function must be declares before the 3 are
** defined.
*/

bool parse_parenthesis(struct arithmetic_lexer *lexer,
    struct arithmetic_ast **ast)
{
    bool res = parse_exp(lexer, ast);

    struct arithmetic_token *token = pop_arithmetic(lexer);
    res = res && token && token->type == TOK_A_RPAR;
    return res;
}

bool parse_texp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    struct arithmetic_token *token = pop_arithmetic(lexer);
    if (!token)
        return false;
    bool res = true;
    if (token->type == TOK_A_LPAR)
        res = parse_parenthesis(lexer, ast);
    else if (token->type == TOK_A_NUMBER)
        *ast = ast_alloc_number(token->value);
    else if (token->type == TOK_A_MINUS)
    {
        *ast = ast_alloc();
        (*ast)->type = EXPR_SUBTRACTION;
        (*ast)->data.children.left = ast_alloc_number(0);
        if (!parse_texp(lexer, &(*ast)->data.children.right))
            return false;
    }
    else if (token->type == TOK_A_PLUS)
    {
        *ast = ast_alloc();
        (*ast)->type = EXPR_ADDITION;
        (*ast)->data.children.left = ast_alloc_number(0);
        if (!parse_texp(lexer, &(*ast)->data.children.right))
            return false;
    }
    else
        res = false;
    return res;
}

bool parse_log_not(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (token->type == TOK_A_NOT)
    {
        *ast = ast_alloc();
        (*ast)->type = EXPR_NOT;
    }
    else if (token->type == TOK_A_TILDE)
    {
        *ast = ast_alloc();
        (*ast)->type = EXPR_TILDE;
    }
    else
        return parse_texp(lexer, ast);
    pop_arithmetic(lexer);
    if (!parse_texp(lexer, &(*ast)->data.children.left))
        return false;
    return true;
}

bool parse_exponent(struct arithmetic_lexer *lexer,
    struct arithmetic_ast **ast)
{
    if (!parse_log_not(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_POW)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_POW;

        res = parse_log_not(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_sexp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_exponent(lexer, ast))
        return false;

    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    while (res && token && token_is_sop(token))
    {
        token = pop_arithmetic(lexer);

        struct arithmetic_ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOK_A_MULTIPLY:
            tmp->type = EXPR_MULTIPLICATION;
            break;
        case TOK_A_DIVIDE:
            tmp->type = EXPR_DIVISION;
            break;
        case TOK_A_POW:
            tmp->type = EXPR_POW;
            break;
        default:
            res = false;
            break;
        }
        res = parse_exponent(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    };

    return res;
}

bool parse_exp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_sexp(lexer, ast))
        return false;

    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    while (res && token && token_is_eop(token))
    {
        token = pop_arithmetic(lexer);

        struct arithmetic_ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOK_A_PLUS:
            tmp->type = EXPR_ADDITION;
            break;
        case TOK_A_MINUS:
            tmp->type = EXPR_SUBTRACTION;
            break;
        default:
            res = false;
            break;
        }
        res = parse_sexp(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    };

    return res;
}

bool parse_bit_and(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_exp(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_SEPAND)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_SEPAND;

        res = parse_exp(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_bit_xor(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_bit_and(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_XOR)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_XOR;

        res = parse_bit_and(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_bit_or(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_bit_xor(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_PIPE)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_PIPE;

        res = parse_bit_xor(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_log_and(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_bit_or(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_AND)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_AND;

        res = parse_bit_or(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_log_or(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_log_and(lexer, ast))
        return false;
    bool res = true;
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token)
        return false;
    while (res && token && token->type == TOK_A_OR)
    {
        token = pop_arithmetic(lexer);
        struct arithmetic_ast *tmp = ast_alloc();
        tmp->type = EXPR_OR;

        res = parse_log_and(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token = peek_arithmetic(lexer);
    }
    return res;
}

bool parse_expression(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token || token->type == TOK_A_END)
        return false;

    if (!parse_log_or(lexer, ast))
        return false;

    token = peek_arithmetic(lexer);
    return token && token->type == TOK_A_END;
}
