#include "../ast/arithmetic_ast.h"
#include "../lexer/arithmetic_lexer.h"
#include "../parser/arithmetic_parser.h"
#include "./arithmetic_ast_print.h"

#include <err.h>
#include <stdio.h>

struct
{
    enum arithmetic_ast_type type;
    char c;
} ast_char[EXPR_NUMBER] =
{
    {EXPR_ADDITION, '+'},
    {EXPR_SUBTRACTION, '-'},
    {EXPR_MULTIPLICATION, '*'},
    {EXPR_DIVISION, '/'}
};

void print_arithmetic_ast(struct arithmetic_ast *ast)
{
    if (!ast)
        return;

    if (ast->type == EXPR_NUMBER)
    {
        printf("%d", ast->data.value);
        return;
    }

    printf("(");
    print_arithmetic_ast(ast->data.children.left);
    for (size_t i = 0; i < sizeof(ast_char) / sizeof(*ast_char); ++i)
        if (ast_char[i].type == ast->type)
        {
            printf("%c", ast_char[i].c);
            break;
        }
    print_arithmetic_ast(ast->data.children.right);
    printf(")");
}

// int main(int argc, char *argv[])
// {
//     if (argc < 2)
//     {
//         puts("Usage: ./test \"expression\"");
//         return 1;
//     }


//     struct lexer *lexer = lexer_alloc(argv[1]);

//     struct arithmetic_ast *ast = NULL;

//     if (parse_expression(lexer, &ast))
//     {
//         print_ast(ast);
//         puts("");
//     }
//     else
//         puts("Parsing error");

//     ast_free(ast);
//     lexer_free(lexer);
// }
