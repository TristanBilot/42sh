#include "ast/ast.h"
#include "utils/xalloc.h"

#include <stdlib.h>
#include <string.h>

struct ast *ast_alloc(void)
{
    struct ast *res = xmalloc(sizeof(*res));
    memset(res, 0, sizeof(*res));
    return res;
}

void ast_free(struct ast *ast)
{
    if (ast && ast->type != EXPR_NUMBER)
    {
        ast_free(right_child(ast));
        ast_free(left_child(ast));
    }

    free(ast);
}

struct ast *ast_alloc_number(int value)
{
    struct ast *res = ast_alloc();
    res->type = EXPR_NUMBER;
    res->data.value = value;
    return res;
}
