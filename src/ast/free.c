#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "free.h"
#include "ast/ast.h"

#define AST_EXISTS(ast) \
    if (!ast)\
        return;

#define FREE_AST(ast) \
    free(ast); \
    ast = NULL;

#define DEBUG_FLAG false
#define DEBUG(msg) if (DEBUG_FLAG) \
                        printf("%s", msg);


void free_input(struct node_input *ast)
{
    DEBUG("free_input\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_and_or(struct node_and_or *ast)
{
    DEBUG("free_andor\n");
    AST_EXISTS(ast);
    free_pipeline(ast->right);
    if (ast->left.pipeline)
        free_pipeline(ast->left.pipeline);
    else if (ast->left.and_or)
        free_and_or(ast->left.and_or);
    FREE_AST(ast);
}

void free_redirection(struct node_redirection *ast)
{
    DEBUG("free_redirection\n");
    AST_EXISTS(ast);
    struct node_redirection *tmp = NULL;
    while (ast)
    {
        tmp = ast->next;
        free(ast);
        ast = tmp;
    }
    ast = NULL;
}

void free_prefix(struct node_prefix *ast)
{
    DEBUG("free_prefix\n");
    AST_EXISTS(ast);
    free_prefix(ast->next);
    if (ast->type == REDIRECTION)
        free_redirection(ast->prefix.redirection);
    FREE_AST(ast);
}

void free_element(struct node_element *ast)
{
    DEBUG("free_element\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_until(struct node_until *ast)
{
    DEBUG("free_until\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_if(struct node_if *ast)
{
    DEBUG("free_if\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_else_clause(struct node_else_clause *ast)
{
    DEBUG("free_else\n");
    AST_EXISTS(ast);
    if (ast->clause.elif)
        free_if(ast->clause.elif);
    else if (ast->clause.else_body)
        free_compound_list(ast->clause.else_body);
    FREE_AST(ast);
}

void free_do_group(struct node_do_group *ast)
{
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_case_clause(struct node_case_clause *ast)
{
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_case_item(struct node_case_item *ast)
{
    AST_EXISTS(ast);
    /*
    if (ast->type == COMPOUND)
        free_compound_list(ast->next.compound_list);
    if (ast->type == NEXT)
        free_case_item(ast->next.next);
    */
    FREE_AST(ast);
}

void free_command(struct node_command *ast)
{
    AST_EXISTS(ast);
    /*
    if (ast->type == SIMPLE_COMMAND)
        free_simple_command(ast->command.simple_command);
    if (ast->type == SHELL_COMMAND)
        free_shell_command(ast->command.shell_command);
    if (ast->type == FUNCDEC)
        free_funcdec(ast->command.funcdec);
    */
    FREE_AST(ast);       
}

void free_simple_command(struct node_simple_command *ast)
{
    AST_EXISTS(ast);
    struct node_prefix *prefix = ast->prefixes;
    struct node_prefix *tmp = NULL;
    while (prefix)
    {
        tmp = prefix;
        prefix = prefix->next;
        free_prefix(tmp);
    }
    struct node_element *element = ast->elements;
    struct node_element *temp = NULL;
    while (element)
    {
        temp = element;
        element = element->next;
        free_element(temp);
    }
    FREE_AST(ast);
}

void free_pipeline(struct node_pipeline *ast)
{
    DEBUG("free_pipeline\n");
    AST_EXISTS(ast);
    //free_command(ast->command);
    //free_pipeline(ast->next_sibling);
    FREE_AST(ast);        
}

void free_list(struct node_list *ast)
{
    DEBUG("free_list\n");
    AST_EXISTS(ast);
    //free_and_or(ast->and_or);
    //free_list(ast->next_sibling);
    FREE_AST(ast);
}

void free_shell_command(struct node_shell_command *ast)
{
    DEBUG("free_shell_command\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_compound_list(struct node_compound_list *ast)
{
    AST_EXISTS(ast);
    FREE_AST(ast);  
}

void free_range(struct range *range)
{
    struct range *tmp = NULL;
    while (range)
    {
        tmp = range;
        range = range->next;
        free(tmp);
    }
}

void free_for(struct node_for *ast)
{
    DEBUG("free_for\n");
    AST_EXISTS(ast);
    if (ast->range)
        free_range(ast->range); // m ca va pas planter ici ? psk t'as encore ast-> range = NULL on teste
    FREE_AST(ast);
}

void free_while(struct node_while *ast)
{
    DEBUG("free_while\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_case(struct node_case *ast)
{
    DEBUG("free_case\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}

void free_funcdec(struct node_funcdec *ast)
{
    DEBUG("free_funcdec\n");
    AST_EXISTS(ast);
    FREE_AST(ast);
}
