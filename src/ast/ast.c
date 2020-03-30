#include <stdio.h>
#include <stdlib.h>

#include "../lexer/token.h"

struct ast_node *ast_node_init(void)
{
    struct ast_node *res = malloc(sizeof(struct ast_node));
    if(!res)
        return NULL;
    return res;
}

void ast_node_free(struct ast_node *node)
{
    if(node != NULL)
        free(node);
}    if(node != NULL)
        free(node);
}    if(node != NULL)
        free(node);
}    if(node != NULL)
        free(node);
}