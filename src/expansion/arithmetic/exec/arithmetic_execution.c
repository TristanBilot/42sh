#include "../exec/arithmetic_execution.h"

double exec_arithmetic_ast(struct arithmetic_ast *ast, bool *error)
{
    if (!ast)
        return true;

    switch (ast->type)
    {
    case EXPR_ADDITION:
        return exec_arithmetic_ast(ast->data.children.left, error) + exec_arithmetic_ast(ast->data.children.right, error);

    case EXPR_SUBTRACTION:
        return exec_arithmetic_ast(ast->data.children.left, error) - exec_arithmetic_ast(ast->data.children.right, error);

    case EXPR_MULTIPLICATION:
        return exec_arithmetic_ast(ast->data.children.left, error) * exec_arithmetic_ast(ast->data.children.right, error);

    case EXPR_DIVISION:
        if (ast->data.children.left->data.value == 0)
        {
            *error = true;
            return 0;
        }
        return exec_arithmetic_ast(ast->data.children.left, error) / exec_arithmetic_ast(ast->data.children.right, error);
    case EXPR_NUMBER:
        return ast->data.value;
    default:
        return 0;
    }
}
