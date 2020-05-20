#ifndef ARITHMETIC_AST_H
#define ARITHMETIC_AST_H

// #include "utils/attr.h"

enum arithmetic_ast_type
{
    EXPR_ADDITION = 0,
    EXPR_SUBTRACTION,
    EXPR_MULTIPLICATION,
    EXPR_DIVISION,
    EXPR_NUMBER
};

struct arithmetic_ast
{
    enum arithmetic_ast_type type;
    union
    {
        struct
        {
            struct arithmetic_ast *left;
            struct arithmetic_ast *right;
        } children;
        int value;
    } data;
};

static inline struct arithmetic_ast *left_child(struct arithmetic_ast *ast)
{
    return ast->data.children.left;
}

static inline struct arithmetic_ast *right_child(struct arithmetic_ast *ast)
{
    return ast->data.children.right;
}

/**
** \brief Ast node allocator and initialiser
**
** \return a pointer to the allocated ast node
*/
struct arithmetic_ast *ast_alloc(void);

/**
** \brief Number ast node allocator and initialiser
**
** \param value the value to store inthe node
** \return a pointer to the allocated ast node
*/
struct arithmetic_ast *ast_alloc_number(int value);

/**
** \brief Wrapper to release memory of an ast node and its children
**
** \param ast the node to free
*/
void ast_free(struct arithmetic_ast *ast);

#endif /* ! ARITHMETIC_AST_H */
