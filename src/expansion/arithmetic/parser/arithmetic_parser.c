#include "../lexer/arithmetic_lexer.h"
#include "../ast/arithmetic_ast.h"
#include "arithmetic_parser.h"
#include "../../../utils/xalloc.h"

static bool token_is_sop(struct arithmetic_token *token)
{
    return token->type == TOK_A_MULTIPLY || token->type == TOK_A_DIVIDE;
}

static bool token_is_eop(struct arithmetic_token *token)
{
    return token->type == TOK_A_PLUS || token->type == TOK_A_MINUS;
}

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
static bool parse_exp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);

static bool parse_parenthesis(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    bool res = parse_exp(lexer, ast);

    struct arithmetic_token *token = pop_arithmetic(lexer);
    res = res && token && token->type == TOK_A_RPAR;

    // token_free(token);
    return res;
}

static bool parse_texp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    struct arithmetic_token *token = pop_arithmetic(lexer);
    if (!token)
        return false;

    bool res = true;
    if (token->type == TOK_A_LPAR)
        res = parse_parenthesis(lexer, ast);
    else if (token->type == TOK_A_NUMBER)
        *ast  = ast_alloc_number(token->value);
    else if (token->type == TOK_A_MINUS)
    {
        // token_free(token);
        token = pop_arithmetic(lexer);

        if (token && token->type == TOK_A_NUMBER)
            *ast = ast_alloc_number(-token->value);
        else
            res = false;
    }
    else
        res = false;

    // token_free(token);
    return res;
}

static bool parse_sexp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    if (!parse_texp(lexer, ast))
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
        default:
            res = false;
            break;
        }
        res = parse_texp(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        // token_free(token);
        token = peek_arithmetic(lexer);
    };

    return res;
}

static bool parse_exp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
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
        // token_free(token);
        token = peek_arithmetic(lexer);
    };

    return res;
}

bool parse_expression(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast)
{
    struct arithmetic_token *token = peek_arithmetic(lexer);
    if (!token || token->type == TOK_A_END)
        return false;

    if (!parse_exp(lexer, ast))
        return false;

    token = peek_arithmetic(lexer);
    return token && token->type == TOK_A_END;
}
