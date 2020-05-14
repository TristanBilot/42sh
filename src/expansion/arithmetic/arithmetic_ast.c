#include "arithmetic_ast.h"
#include "../utils/xalloc.h"

struct ast *new_arithmetic_ast(void)
{
    struct arithmetic_ast *new = xcalloc(1, sizeof(struct arithmetic_ast));
    memset(new, 0, sizeof(*new));
    return new;
}

struct arithmetic_ast *left_child(struct arithmetic_ast *ast)
{
    return ast->data.children.left;
}

struct arithmetic_ast *right_child(struct arithmetic_ast *ast)
{
    return ast->data.children.right;
}
