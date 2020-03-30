#ifndef AST_H
#define AST_H

#include <stdbool.h>

/*struct ast_node
{
    enum node_type
    {
        NODE_IF,                                // = KEYWORD
        NODE_SIMPLECOMMAND,                     // = TOKEN COMMAND
        NODE_WORD,                              // = WORD
        NODE_LOGICAL
    } type;

    union
    {
        struct ast_node_if *ast_node_if;
        struct ast_node_command *ast_node_simplecommand;
        struct ast_node_word *ast_node_word;
        struct ast_node_logical *ast_node_logical;
    } data;
};*/

struct node_input{
    struct node_list *node_list;
};

struct node_list{
    struct node_and_or *and_or;
};

struct node_and_or
{
    struct node_pipeline *left;
    struct node_pipeline *right;
};

struct node_pipeline
{
    bool truc_de_merde;
};
struct node_command
{
    struct node_simple_command *command;
    struct node_shell_command *shell_command;
    struct node_funcdec *funcdec;
    struct node_redirection *redirection;
};


struct node_simple_command{
    struct node_prefix **prefixes;
};

struct shell_command{
    struct node_compound_list *compound_list;
    struct node_rule_for *rule_for;
    struct node_rule_while *rule_while;
    struct node_rule_until *rule_until;
    struct node_rule_case *rule_case;
    struct node_rule_if *rule_if;
};


struct funcdec
{
    bool is_function;
    char *function_name;
    // struct node_shell_command *shell_command;
};


struct node_compound_list
{
    struct ast_node **list;
    struct ast_node *current_node;
};

struct node_if                              // INSTANCE KEYWORD
{
    struct node_compound_list *condition;   // the condition
    struct node_compound_list *if_body;   // on peut mettre directement ici la liste de struct elif  // the body of the if
    struct node_else_clause *else_clause;   // the body of the else, may be NULL
};

struct ast_node_else_clause
{
    union
    {
        struct ast_node_if *elif; 
        struct ast_node_compound_list *else_body;
    } data;
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

/**
** \brief Ast node allocator and initialiser
**
** \return a pointer to the allocated ast node
*/
struct ast_node *ast_node_init(int type);

/**
** \brief Wrapper to release memory of an ast node and its children
**
** \param node the node to free
*/

void ast_node_free(struct ast_node *node);



/**
** \brief Ast node_if allocator and initialiser
**
** \return a pointer to the allocated ast node_if
*/
struct ast_node_if *ast_node_if_init(void);

/**
** \brief Wrapper to release memory of an ast node_if and its children
**
** \param node the node to free
*/

void ast_node_if_free(struct ast_node_if *node);


/**
** \brief Ast node_command allocator and initialiser
**
** \return a pointer to the allocated ast node_command
*/
struct ast_node_command *ast_node_command_init(void);

/**
** \brief Wrapper to release memory of an ast node_command and its children
**
** \param node_command the node to free
*/

void ast_node_command_free(struct ast_node_command *node);


/**
** \brief Ast node_word allocator and initialiser
**
** \return a pointer to the allocated ast node_word
*/
struct ast_node_word *ast_node_word_init(void);

/**
** \brief Wrapper to release memory of an ast node_word and its children
**
** \param node the node to free
*/

void ast_node_word_free(struct ast_node_word *node);


/**
** \brief Ast node_logical allocator and initialiser
**
** \return a pointer to the allocated ast node_logical
*/
struct ast_node_logical *ast_node_logical_init(void);

/**
** \brief Wrapper to release memory of an ast node_logical and its children
**
** \param node the node to free
*/

void ast_node_logical_free(struct ast_node_logical *node);

#endif /* ! AST_H */
