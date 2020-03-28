#include <stdio.h>
#include <stdlib.h>
#include <../lexer/token.h>

struct ast_node
{
    enum node_type
    {
        NODE_IF,                                // = KEYWORD
        NODE_SIMPLECOMMAND,                     // = TOKEN COMMAND
        NODE_WORD                               // = WORD
    } type;

    union
    {
        struct ast_node_if ast_node_if;
        struct ast_node_command ast_node_simplecommand;
        struct ast_node_word;
        struct ast_node_logical;
    } data;
};

struct ast_node *ast_node_alloc(void)
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
}