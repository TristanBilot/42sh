#include <stddef.h>
#include <stdlib.h>
#include "free.h"
#include "ast/ast.h"

#define AST_EXISTS(ast) \
    if (!ast)\
        return;

#define FREE_AST(ast) \
    free(ast); \
    ast = NULL;

void free_input(struct node_input *ast)
{
    AST_EXISTS(ast);
    free_list(ast->node_list);
    FREE_AST(ast);
}

void free_and_or(struct node_and_or *ast)
{
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
    AST_EXISTS(ast);
    free_prefix(ast->next);
    if (ast->type == REDIRECTION)
        free_redirection(ast->prefix.redirection);
    FREE_AST(ast);
}

void free_element(struct node_element *ast)
{
    AST_EXISTS(ast);
    free_element(ast->next);
    if (ast->type == TOKEN_REDIRECTION)
        free_redirection(ast->element.redirection);
    FREE_AST(ast);
}

void free_until(struct node_until *ast)
{
    AST_EXISTS(ast);
    free_compound_list(ast->condition);
    free_do_group(ast->body);
    FREE_AST(ast);
}

void free_if(struct node_if *ast)
{
    AST_EXISTS(ast);
    free_compound_list(ast->condition);
    free_compound_list(ast->if_body);
    free_else_clause(ast->else_clause);
    FREE_AST(ast);
}

void free_else_clause(struct node_else_clause *ast)
{
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
    free_compound_list(ast->body);
    FREE_AST(ast);
}

void free_case_clause(struct node_case_clause *ast)
{
    AST_EXISTS(ast);
    free_case_item(ast->case_item);
    free_case_clause(ast->next);
    FREE_AST(ast);
}

void free_case_item(struct node_case_item *ast)
{
    AST_EXISTS(ast);
    if (ast->type == COMPOUND)
        free_compound_list(ast->next.compound_list);
    if (ast->type == NEXT)
        free_case_item(ast->next.next);
    FREE_AST(ast);
}

void free_command(struct node_command *ast)
{
    AST_EXISTS(ast);
    if (ast->type == SIMPLE_COMMAND)
        free_simple_command(ast->command.simple_command);
    if (ast->type == SHELL_COMMAND)
        free_shell_command(ast->command.shell_command);
    if (ast->type == FUNCDEC)
        free_funcdec(ast->command.funcdec);
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
    AST_EXISTS(ast);
    free_command(ast->command);
    free_pipeline(ast->next_sibling);
    FREE_AST(ast);        
}

void free_list(struct node_list *ast)
{
    AST_EXISTS(ast);
    free_and_or(ast->and_or);
    free_list(ast->next_sibling);
    FREE_AST(ast);
}

void free_shell_command(struct node_shell_command *ast)
{
    AST_EXISTS(ast);
    if (ast->shell.compound_list)
        free_compound_list(ast->shell.compound_list);
    if (ast->shell.rule_for)
        free_for(ast->shell.rule_for);
    if (ast->shell.rule_while)
        free_while(ast->shell.rule_while);
    if (ast->shell.rule_until)
        free_until(ast->shell.rule_until);
    if (ast->shell.rule_case)
        free_case(ast->shell.rule_case);
    if (ast->shell.rule_if)
        free_if(ast->shell.rule_if);
    FREE_AST(ast);
}

void free_compound_list(struct node_compound_list *ast)
{
    AST_EXISTS(ast);
    free_and_or(ast->and_or);
    free_compound_list(ast->next_sibling);  
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
    AST_EXISTS(ast);
    //int i = 0;
    //char *tmp = NULL;
    // printf("aaaaaaaaaaaaaaaaaaa => %p\n", ast);
    // while (ast->range && ast->range[i])
    // {
    //     tmp = ast->range[i++];
    //     free(tmp);
    // }
    if (ast->range)
        free_range(ast->range); // m ca va pas planter ici ? psk t'as encore ast-> range = NULL on teste
    // free(ast->range);
    free_do_group(ast->body);
    FREE_AST(ast);
}

void free_while(struct node_while *ast)
{
    AST_EXISTS(ast);
    free_compound_list(ast->condition);
    free_do_group(ast->body);
    FREE_AST(ast);
}

void free_case(struct node_case *ast)
{
    AST_EXISTS(ast);
    free_case_clause(ast->case_clause);
    FREE_AST(ast);
}

void free_funcdec(struct node_funcdec *ast)
{
    AST_EXISTS(ast);
    free_shell_command(ast->shell_command);
    FREE_AST(ast);
}
