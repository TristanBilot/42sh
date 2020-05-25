#include "../exec/arithmetic_execution.h"

int	my_pow(int n, int p)
{
    int count;
    int res;
    count = 1;
    res = 1;
    if (p < 0)
        return 0;
    if (p == 0)
        return 1;
    while (count <= p)
    {
        res = res * n;
        count = count + 1;
    }
    return res;
}

int to_int(double x)
{
    return x;
}

double exec_arithmetic_ast(struct arithmetic_ast *ast, bool *error)
{
    if (!ast)
        return true;

    switch (ast->type)
    {
    case EXPR_ADDITION:
        return exec_arithmetic_ast(ast->data.children.left, error)
            + exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_SUBTRACTION:
        return exec_arithmetic_ast(ast->data.children.left, error)
            - exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_MULTIPLICATION:
        return exec_arithmetic_ast(ast->data.children.left, error)
            * exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_DIVISION:
        if (ast->data.children.right->data.value == 0)
        {
            *error = true;
            return 0;
        }
        return exec_arithmetic_ast(ast->data.children.left, error)
            / exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_POW:
        return my_pow(exec_arithmetic_ast(ast->data.children.left, error),
            exec_arithmetic_ast(ast->data.children.right, error));
    case EXPR_SEPAND:
        return to_int(exec_arithmetic_ast(ast->data.children.left, error))
            & to_int(exec_arithmetic_ast(ast->data.children.right, error));
    case EXPR_PIPE:
        return to_int(exec_arithmetic_ast(ast->data.children.left, error))
            | to_int(exec_arithmetic_ast(ast->data.children.right, error));
    case EXPR_XOR:
        return to_int(exec_arithmetic_ast(ast->data.children.left, error))
            ^ to_int(exec_arithmetic_ast(ast->data.children.right, error));
    case EXPR_AND:
        return exec_arithmetic_ast(ast->data.children.left, error)
            && exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_OR:
        return exec_arithmetic_ast(ast->data.children.left, error)
            || exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_NOT:
        return !to_int(exec_arithmetic_ast(ast->data.children.left, error));
    case EXPR_TILDE:
        return ~to_int(exec_arithmetic_ast(ast->data.children.left, error));
    case EXPR_MINUS_EQ:
        return exec_arithmetic_ast(ast->data.children.left, error)
            || exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_NUMBER:
        return ast->data.value;
    default:
        return 0;
    }
}
