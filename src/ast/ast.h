#ifndef AST_H
#define AST_H

#include <stdbool.h>

// struct ast_node
// {
//     enum node_type
//     {
//         NODE_IF,                                // = KEYWORD
//         NODE_SIMPLECOMMAND,                     // = TOKEN COMMAND
//         NODE_WORD,                              // = WORD
//         NODE_LOGICAL
//     } type;

//     union
//     {
//         struct ast_node_if *ast_node_if;
//         struct ast_node_command *ast_node_simplecommand;
//         struct ast_node_word *ast_node_word;
//         struct ast_node_logical *ast_node_logical;
//     } data;
// };

struct node_input{
    struct node_list *node_list;
};

struct node_list{
    struct node_and_or **and_or_list;
    enum type{
        SEMI,
        SEPAND
    };
};

struct node_and_or
{
    struct node_pipeline **pipelines;

    enum type{
        AND,
        OR
    };
};

struct node_pipeline
{
    bool is_not;
    struct node_command **commands;
};
struct node_command
{
    union command
    {
        struct node_simple_command *command;
        struct node_shell_command *shell_command;
        struct node_funcdec *funcdec;
    };
    struct node_redirection **redirections;
};

struct node_simple_command
{
    struct node_prefix **prefixes;
    struct node_element **elements;
};

struct node_shell_command
{
    union shell
    {
        struct node_compound_list *compound_list;
        struct node_rule_for *rule_for;
        struct node_rule_while *rule_while;
        struct node_rule_until *rule_until;
        struct node_rule_case *rule_case;
        struct node_rule_if *rule_if;
    };
};


struct node_funcdec
{
    bool is_function;
    char *function_name;
    struct node_shell_command *shell_command;
};

struct node_redirection
{
    enum token_type type;
    char *left;
    char *right;
};

struct node_prefix
{
    union prefix
    {
        struct assigment_word
        {
            char *variable_name;
            char *value;
        };
        struct node_redirection;
    };
};

struct node_element
{
    union element
    {
        char *word;
        struct node_redirection;
    };
};

struct node_compound_list
{
    struct ast_node **list;
    struct ast_node *current_node;
};

struct node_while
{
    struct node_compound_list *compound_list;
    struct node_compound_list *do_group;
};

struct node_until
{
    struct node_compound_list *compound_list;
    struct node_compound_list *do_group;
};

struct node_rule_case
{
    char *word;
    struct node_case_clause *case_clause;
};

struct node_if                              // INSTANCE KEYWORD
{
    struct node_compound_list *condition;   // the condition
    struct node_compound_list *if_body;   // on peut mettre directement ici la liste de struct elif  // the body of the if
    struct node_else_clause *else_clause;   // the body of the else, may be NULL
};

struct node_else_clause
{
    union
    {
        struct ast_node_if *elif; 
        struct ast_node_compound_list *else_body;
    } data;
};

struct node_case_clause
{
    struct node_case_item **case_items;
};

struct node_case_item
{
    char **words;
    struct node_compound_list *compound_list;
};

/*struct ast_node_logical                         //INSTANCE LOGICAL
{
};

struct ast_node_word
{
};

struct ast_node_bracket
{
};

struct ast_node_parenthesis
{
};

struct ast_node_if
{
};

struct ast_node_for
{
};

struct ast_node_while
{
};
*/

