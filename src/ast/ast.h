#ifndef AST_H
#define AST_H


struct ast_node_if                              // INSTANCE KEYWORD
{
    struct ast_node_compound_list *condition;   // the condition
    struct ast_node_compound_list *if_body;     // the body of the if
    struct ast_node_else_clause *else_clause;   // the body of the else, may be NULL
};


struct ast_node_logical                         //INSTANCE LOGICAL
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


/**
** \brief Ast node allocator and initialiser
**
** \return a pointer to the allocated ast node
*/
struct ast *ast_alloc(void);

/**
** \brief Number ast node allocator and initialiser
**
** \param value the value to store inthe node
** \return a pointer to the allocated ast node
*/
struct ast *ast_alloc_number(int value);

/**
** \brief Wrapper to release memory of an ast node and its children
**
** \param ast the node to free
*/
void ast_free(struct ast *ast);

#endif /* ! AST_H */
