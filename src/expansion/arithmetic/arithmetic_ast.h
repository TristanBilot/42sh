#ifndef ARITHMETIC_AST_H
#define ARITHMETIC_AST_H

enum arithmetic_ast_type
{
    NODE_ADD = 0,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_NUM
};

struct arithmetic_ast
{
    enum arithmetic_ast_type type;
    union {
        struct {
            struct arithmetic_ast *left;
            struct arithmetic_ast *right;
        } children;
        int value;
    } data;
};

struct arithmetic_ast *new_arithmetic_ast(void);
struct arithmetic_ast *ast_alloc_number(int value);

struct arithmetic_ast *left_child(struct arithmetic_ast *ast);
struct arithmetic_ast *right_child(struct arithmetic_ast *ast);

#endif /* ARITHMETIC_AST_H */
