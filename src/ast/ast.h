#ifndef AST_H
#define AST_H

#include "utils/attr.h"

enum ast_type
{
    EXPR_ADDITION = 0,
    EXPR_SUBTRACTION,
    EXPR_MULTIPLICATION,
    EXPR_DIVISION,
    EXPR_NUMBER
};

struct ast
{
    enum ast_type type;
    union {
        struct {
            struct ast *left;
            struct ast *right;
        } children;
        int value;
    } data;
};

static inline struct ast *left_child(struct ast *ast)
{
    return ast->data.children.left;
}

static inline struct ast *right_child(struct ast *ast)
{
    return ast->data.children.right;
}

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
