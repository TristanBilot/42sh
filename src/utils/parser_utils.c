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
    char *range;
    if (!ast->prefixes)
    {
        ast->prefixes = prefix;
        return ast->prefixes;
    }
    struct node_prefix *first = ast->prefixes;
    while (first)
    {
        if (!first->next)
        {
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
        ast->elements = element;
        return ast->elements;
    }
    struct node_element *first = ast->elements;
    while (first)
    {
        if (!first->next)
        {
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
        ast->redirections = redirection;
        return ast->redirections;
    }
    struct node_redirection *first = ast->redirections;
    while (first)
    {
        if (!first->next)
        {
            first->next = redirection;
            return ast->redirections;
        }
        first = first->next;
    }
    return ast->redirections;
}
