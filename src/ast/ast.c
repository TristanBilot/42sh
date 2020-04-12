#include <stdio.h>
#include <stdlib.h>

#include "utils/xalloc.h"
#include "parser/parser.h"
#include "ast/ast.h"

struct node_input *build_input(void)                 // \n EOF
{
    struct node_input *new = xmalloc(sizeof(struct node_input));
    new->node_list = NULL;
    if(new)
        printf("NODE CREATE IN BUILD_INPUT NOT NULL");
    else
        printf("ERROR NODE BUILD_INPUT\n");
    return new;
}

struct node_list *build_list(void)                 // ; &
{
    struct node_list *new = xmalloc(sizeof(struct node_list));
    new->next_sibling = NULL;
    new->and_or = NULL;
    return new;
}

struct node_and_or *build_and_or_final(bool is_and, struct node_pipeline *left, struct node_pipeline *right)               // || &&
{
    struct node_and_or *new = xmalloc(sizeof(struct node_and_or));
    new->is_final = true;
    new->left.pipeline = left;
    new->right = right;
    new->type = (is_and ? AND : OR);
    return new;
}

struct node_and_or *build_and_or_merge(bool is_and, struct node_and_or *left, struct node_pipeline *right)               // || &&
{
    struct node_and_or *new = xmalloc(sizeof(struct node_and_or));
    new->is_final = false;
    new->left.and_or = left;
    new->right = right;
    new->type = (is_and ? AND : OR);
    return new;
}

struct node_pipeline *build_pipeline(bool is_not)             // |
{
    struct node_pipeline *new = xmalloc(sizeof(struct node_pipeline));
    new->command = NULL;
    new->next_sibling = NULL;
    new->is_not = is_not;
    return new;
}

struct node_command *build_command(void)             // command
{
    struct node_command *new = xmalloc(sizeof(struct node_command));
    new->command.simple_command = NULL;
    new->redirections = NULL;
    return new;
}

struct node_simple_command *build_simple_command(void)
{
    struct node_simple_command *new = xmalloc(sizeof(struct node_simple_command));
    new->prefixes = NULL; //xmalloc(sizeof(struct node_prefix));
    new->elements = NULL; //xmalloc(sizeof(struct node_element));
    return new;
}

struct node_shell_command *build_shell_command(struct parser *parser)
{
    struct node_shell_command *new = xmalloc(sizeof(struct node_shell_command));
    if (is_type(parser->current_token, TOK_LPAREN))
    {
        new->shell.compound_list = NULL;
        new->type = PARENTHESIS;
    }
    else if (is_type(parser->current_token, TOK_LCURL))
    {
        new->shell.compound_list = NULL;
        new->type = C_BRACKETS;
    }
    else
    {
        new->type = RULE;
    }
    return new;
}

struct node_funcdec *build_funcdec(bool is_function, char *func_name)
{
    struct node_funcdec *new = xmalloc(sizeof(struct node_funcdec));
    new->is_function = is_function;
    new->function_name = func_name;
    new->shell_command = NULL;
    return new;
}


struct node_redirection *build_redirection(struct parser *parser)
{
    struct node_redirection *new = xmalloc(sizeof(struct node_redirection));
    new->left = parser->current_token->value;
    /*next_token(parser);
    int type = parser->current_token->type;
    if (type !=  DLESSDASH && type != DLESS  && type != LESSGREAT
        type !=  LESSAND && type != LESS  && type != DGREAT
        type !=  GREATAND && type != CLOBBER  && type != GREAT)
        return NULL;
    new->type = type;
    next_token(parser);
    new->right = parser->current_token->value;*/
    return new;
    
}


struct node_prefix *build_prefix(struct parser *parser)
{
    struct node_prefix *new = xmalloc(sizeof(struct node_prefix));
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        new->prefix.assigment_word.variable_name = "";
        new->prefix.assigment_word.value = "";
        new->type = ASSIGMENT_WORD;
    }
    else
    {
        new->prefix.redirection = NULL;
        new->type = REDIRECTION;
    }
    return new;
}

struct node_element *build_element(struct parser *parser)
{
    int type = parser->current_token->type;
    struct node_element *new = xmalloc(sizeof(struct node_element));
    switch (type)
    {
    case TOK_IONUMBER:
        new->element.redirection = NULL;
        break;
    case TOK_WORD:
        new->element.word = "";
        break;
    default:
        break;
    }
    return new;
}
 

struct node_compound_list *build_compound_list(void)
{
    struct node_compound_list *new = xmalloc(sizeof(struct node_compound_list));
    new->and_or = NULL;
    new->next_sibling = NULL;
    return new;
}

struct node_while *build_while(void)
{
    struct node_while *new = xmalloc(sizeof(struct node_while));
    new->condition = NULL;
    new->body = NULL;
    return new;
}

struct node_until *build_until(void)
{
    struct node_until *new = xmalloc(sizeof(struct node_until));
    new->condition = NULL;
    new->body = NULL;
    return new;
}

struct node_case *build_case(struct parser *parser)
{
    struct node_case *new = xmalloc(sizeof(struct node_case));
    new->word = parser->current_token->value;
    new->case_clause = NULL;
    return new;
}

struct node_if *build_if(void)
{
    struct node_if *new = xmalloc(sizeof(struct node_if));
    new->condition = NULL;
    new->if_body = NULL;
    new->else_clause = NULL;
    return new;
} 

struct node_for *build_for(void)
{
    struct node_for *new = xmalloc(sizeof(struct node_for));
    new->variable_name = "";
    new->range = NULL;
    new->body = NULL;
    return new;
}

struct node_else_clause *build_else_clause(struct parser *parser)
{
    struct node_else_clause *new = xmalloc(sizeof(struct node_else_clause));
    if (is_type(parser->current_token, KW_ELIF))
    {
        new->type = ELIF;
        new->clause.elif = NULL;
    }
    else
    {
        new->type = ELSE;
        new->clause.else_body = NULL;
    }
    return new;
}

struct node_do_group *build_do_group(void)
{
    struct node_do_group *new = xmalloc(sizeof(struct node_do_group));
    new->body = NULL;
    return new;
}

struct node_case_clause *build_case_clause(void)     // ; &
{
    struct node_case_clause *new = xmalloc(sizeof(struct node_case_clause));
    new->case_item = NULL;
    new->next = NULL;
    return new;
}

struct node_case_item *build_case_item(struct parser *parser)
{
    struct node_case_item *new = xmalloc(sizeof(struct node_case_item));
    new->word = parser->current_token->value;
    return new;
}

