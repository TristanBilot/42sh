#include <stdio.h>
#include <stdlib.h>
#include <ast/ast.h>
#include "lexer/token.h"



int is_type(struct ast_node *node, int type)
{
    return node->type == type;
}

struct ast_node *ast_node_init(int type)
{
    struct ast_node *res = malloc(sizeof(struct ast_node));
    if (!res)
        return NULL;
    res->type = type;
    if (type == NODE_IF)
        res->data.ast_node_if = ast_node_if_init();
    if (type == NODE_LOGICAL)
        res->data.ast_node_logical = ast_node_logical_init();           // = On met tout sur un if comme ça ca va initialiser le reste à NULL
    if (type == NODE_WORD)
        res->data.ast_node_word = ast_node_word_init();                // = ou sinon tout à NULL si il ne trouve pas
    if (type == NODE_SIMPLECOMMAND)
        res->data.ast_node_simplecommand = ast_node_command_init();
    return res;
}


void ast_node_free(struct ast_node *node)
{
    if (node != NULL)
        free(node);
}

struct ast_node_if *ast_node_if_init(void)
{
    struct ast_node_if *node = malloc(sizeof(struct ast_node_if));
    if (!node)
        return true;
    }
    if (parse_prefix(parser, ast))
    {
        do
        return NULL;
    return node;
}

void ast_node_if_free(st
        return true;
    }
    if (parse_prefix(parser, ast))
    {
        doruct ast_node_if *node){
    if (node)
        free(node);
}

struct ast_node_command *ast_node_command_init(void)
{
    struct ast_node_command *node = malloc(sizeof(struct ast_node_command));
    if (!node)
        return NULL;
    return node;
}



void ast_node_command_free(struct ast_node_command *node)
{
    if (node)
        free(node);
}



struct ast_node_word *ast_node_word_init(void)
{
    struct ast_node_word *node = malloc(sizeof(struct ast_node_word));
    if (!node)
        return NULL;
    return node;
}



void ast_node_word_free(struct ast_node_word *node)
{
    if (node)
        free(node);
}


struct ast_node_logical *ast_node_logical_init(void)
{
    struct ast_node_logical *node = malloc(sizeof(struct ast_node_logical));
    if (!node)
        return NULL;
    return node;
}



void ast_node_logical_free(struct ast_node_logical *node)
{
    if (node)
        free(node);
}