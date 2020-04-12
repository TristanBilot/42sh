#include <stdlib.h>
#include "../lexer/token.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "parser_utils.h"

bool is_redirection(struct token *token)
{
    return (is_type(token, TOK_DLESSDASH) ||
        is_type(token, TOK_DLESS) ||
        is_type(token, TOK_LESSGREAT) ||
        is_type(token, TOK_LESSAND) ||
        is_type(token, TOK_LESS) ||
        is_type(token, TOK_DGREAT) ||
        is_type(token, TOK_GREATAND) ||
        is_type(token, TOK_CLOBBER) ||
        is_type(token, TOK_GREAT));
}

struct node_prefix *append_prefix(
    struct node_simple_command *ast,
    struct node_prefix *prefix) 
{
    if (!ast)
        return NULL;
    if (!ast->prefixes)
    {
        prefix->next = NULL;
        ast->prefixes = prefix;
        return ast->prefixes;
    }
    struct node_prefix *first = ast->prefixes;
    while (first)
    {
        if (!first->next)
        {
            if (prefix)
                prefix->next = NULL;
            first->next = prefix;
            return ast->prefixes;
        }
        first = first->next;
    }
    return ast->prefixes;
}

struct node_element *append_element(
    struct node_simple_command *ast,
    struct node_element *element)
{
    if (!ast)
        return NULL;
    if (!ast->elements)
    {
        element->next = NULL;
        ast->elements = element;
        return ast->elements;
    }
    struct node_element *first = ast->elements;
    while (first)
    {
        if (!first->next)
        {
            if (element)
                element->next = NULL;
            first->next = element;
            return ast->elements;
        }
        first = first->next;
    }
    return ast->elements;
}

struct node_redirection *append_redirection(
    struct node_command *ast,
    struct node_redirection *redirection) 
{
    if (!ast)
        return NULL;
    if (!ast->redirections)
    {
        redirection->next = NULL;
        ast->redirections = redirection;
        return ast->redirections;
    }
    struct node_redirection *first = ast->redirections;
    while (first)
    {
        if (!first->next)
        {
            if (redirection)
                redirection->next = NULL;
            first->next = redirection;
            return ast->redirections;
        }
        first = first->next;
    }
    return ast->redirections;
}

struct range *append_value_to_for(
    struct node_for *ast,
    char *value) 
{
    if (!ast)
        return NULL;
    struct range *new = malloc(sizeof(struct range));
    new->next = NULL;
    new->value = value;
    if (!ast->range)
    {
        ast->range = new;
        return ast->range;
    }
    struct range *first = ast->range;
    while (first)
    {
        if (!first->next)
        {
            first->next = new;
            return ast->range;
        }
        first = first->next;
    }
    return ast->range;
}
