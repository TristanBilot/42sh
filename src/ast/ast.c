#include <stdio.h>
#include <stdlib.h>

#include "utils/xalloc.h"
#include "parser/parser.h"
#include "ast/ast.h"

struct node_input *build_input(void)                 // \n EOF
{
    struct node_input *new = calloc(1, sizeof(struct node_input));
    new->node_list = NULL;
    // printf("INPUT\n");
    return new;
}

struct node_list *build_list(void)                 // ; &
{
    struct node_list *new = calloc(1, sizeof(struct node_list));
    new->next_sibling = NULL;
    new->and_or = NULL;
    // printf("LIST\n");
    return new;
}

struct node_and_or *build_and_or_final(bool is_and, struct node_pipeline *left, struct node_pipeline *right)               // || &&
{
    struct node_and_or *new = calloc(1, sizeof(struct node_and_or));
    new->is_final = true;
    new->left.pipeline = left;
    new->right = right;
    new->type = (is_and ? AND : OR);
    // printf("AND_OR\n");
    return new;
}

struct node_and_or *build_and_or_merge(bool is_and, struct node_and_or *left, struct node_pipeline *right)               // || &&
{
    struct node_and_or *new = calloc(1, sizeof(struct node_and_or));
    new->is_final = false;
    new->left.and_or = left;
    new->right = right;
    new->type = (is_and ? AND : OR);
    // printf("AND_OR\n");
    return new;
}

struct node_pipeline *build_pipeline(bool is_not)             // |
{
    struct node_pipeline *new = calloc(1, sizeof(struct node_pipeline));
    new->command = NULL;
    new->next_sibling = NULL;
    new->is_not = is_not;
    // printf("PIPELINE\n");
    return new;
}

struct node_command *build_command(void)             // command
{
    struct node_command *new = calloc(1, sizeof(struct node_command));
    new->command.simple_command = NULL;
    new->redirections = NULL;
    // printf("COMMAND\n");
    return new;
}

struct node_simple_command *build_simple_command(void)
{
    struct node_simple_command *new = calloc(1, sizeof(struct node_simple_command));
    new->prefixes = NULL; //calloc(1, sizeof(struct node_prefix));
    new->elements = NULL; //calloc(1, sizeof(struct node_element));
    // printf("SIMPLE COMMAND\n");
    return new;
}

struct node_shell_command *build_shell_command(struct parser *parser)
{
    struct node_shell_command *new = calloc(1, sizeof(struct node_shell_command));
    new->shell.compound_list = NULL;
    if (is_type(parser->current_token, TOK_LPAREN))
        new->type = PARENTHESIS;
    else if (is_type(parser->current_token, TOK_LCURL))
        new->type = C_BRACKETS;
    else
        new->type = RULE;
    // printf("SHELL COMMAND \n");
    return new;
}

struct node_funcdec *build_funcdec(bool is_function, char *func_name)
{
    struct node_funcdec *new = calloc(1, sizeof(struct node_funcdec));
    new->is_function = is_function;
    new->function_name = func_name;
    new->shell_command = NULL;
    // printf("FUNCDEC\n");
    return new;
}


struct node_redirection *build_redirection(struct parser *parser)
{
    struct node_redirection *new = calloc(1, sizeof(struct node_redirection));
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
    // printf("REDIRECTION\n");
    return new;
    
}

struct node_prefix *build_prefix(struct parser *parser)
{
    struct node_prefix *new = calloc(1, sizeof(struct node_prefix));
    printf("pointer pref : %p\n", new);
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        new->prefix.assigment_word = calloc(1, sizeof(struct assigment_word));
        new->prefix.assigment_word->variable_name = "";
        new->prefix.assigment_word->value = "";
        new->type = ASSIGMENT_WORD;
    }
    else
    {
        new->prefix.redirection = NULL;
        new->type = REDIRECTION;
    }
    // printf("PREFIX \n");
    return new;
}

struct node_element *build_element(struct parser *parser)
{
    int type = parser->current_token->type;
    struct node_element *new = calloc(1, sizeof(struct node_element));
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
    // printf("ELEMENT\n");
    return new;
}
 

struct node_compound_list *build_compound_list(void)
{
    struct node_compound_list *new = calloc(1, sizeof(struct node_compound_list));
    new->and_or = NULL;
    new->next_sibling = NULL;
    // printf("COMPOUND LIST\n");
    return new;
}

struct node_while *build_while(void)
{
    struct node_while *new = calloc(1, sizeof(struct node_while));
    new->condition = NULL;
    new->body = NULL;
    // printf("WHILE\n");
    return new;
}

struct node_until *build_until(void)
{
    struct node_until *new = calloc(1, sizeof(struct node_until));
    new->condition = NULL;
    new->body = NULL;
    // printf("UNTIL\n");
    return new;
}

struct node_case *build_case(struct parser *parser)
{
    struct node_case *new = calloc(1, sizeof(struct node_case));
    new->word = parser->current_token->value;
    new->case_clause = NULL;
    // printf("CASE\n");
    return new;
}

struct node_if *build_if(void)
{
    struct node_if *new = calloc(1, sizeof(struct node_if));
    new->condition = NULL;
    new->if_body = NULL;
    new->else_clause = NULL;
    // printf("IF\n");
    return new;
} 

struct node_for *build_for(void)
{
    struct node_for *new = calloc(1, sizeof(struct node_for));
    new->variable_name = "";
    new->range = NULL;
    new->body = NULL;
    // printf("FOR\n");
    return new;
}

struct node_else_clause *build_else_clause(struct parser *parser)
{
    struct node_else_clause *new = calloc(1, sizeof(struct node_else_clause));
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
    // printf("ELSE_CLAUSE\n");
    return new;
}

struct node_do_group *build_do_group(void)
{
    struct node_do_group *new = calloc(1, sizeof(struct node_do_group));
    new->body = NULL;
    // printf("DO\n");
    return new;
}

struct node_case_clause *build_case_clause(void)     // ; &
{
    struct node_case_clause *new = calloc(1, sizeof(struct node_case_clause));
    new->case_item = NULL;
    new->next = NULL;
    // printf("CASE_CLAUSE\n");
    return new;
}

struct node_case_item *build_case_item(struct parser *parser)
{
    struct node_case_item *new = calloc(1, sizeof(struct node_case_item));
    new->word = parser->current_token->value;
    // printf("CASE_ITEM\n");
    return new;
}

