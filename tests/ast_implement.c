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

struct ast_node_compound_list
{
    struct ast_node *compound;
};

struct ast_node_if                              // INSTANCE KEYWORD
{
    struct ast_node_compound_list *condition;   // the condition
    struct ast_node_compound_list *if_body;     // the body of the if
    struct ast_node_else_clause *else_clause;   // the body of the else, may be NULL
};
struct ast_node_else_clause
struct ast_node_command                         // INSTANCE COMMAND
{
    struct ast_node_compound_list *commands;
};

struct ast_node_logical                         //INSTANCE LOGICAL
{
    struct ast_node_compound_list *logicals;
};

struct ast_node_word 
{
    struct ast_node_compound_list *word;
};
/*struct ast *ast_alloc_number(int value)
{
    struct ast *res = ast_alloc();
    res->type = EXPR_NUMBER;
    res->data.value = value;
    return res;
}*/


