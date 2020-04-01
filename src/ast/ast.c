#include <stdio.h>
#include <stdlib.h>
#include <ast/ast.h>
#include "lexer/token.h"
#include "../utils/xalloc.h"

struct node_list *build_list(struct parser *parser)
{
    struct node_list *new = xmalloc(sizeof(struct node_list));
    new->and_or_list = xmalloc(sizeof(struct node_and_or));
    return new;
}
