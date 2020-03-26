#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

#include <err.h>
#include <stdio.h>

static struct
    {
    enum ast_type type;
    char c;
    }
ast_char[EXPR_NUMBER] =
{
    {EXPR_ADDITION, '+'},
    {EXPR_SUBTRACTION, '-'},
    {EXPR_MULTIPLICATION, '*'},
    {EXPR_DIVISION, '/'}
};

static void print_ast(struct ast *ast)
{
    if (!ast)
        return;

    if (ast->type == EXPR_NUMBER)
    {
        printf("%d", ast->data.value);
        return;
    }

    printf("(");
    print_ast(ast->data.children.left);
    for (size_t i = 0; i < sizeof(ast_char) / sizeof(*ast_char); ++i)
        if (ast_char[i].type == ast->type)
        {
            printf("%c", ast_char[i].c);
            break;
        }
    print_ast(ast->data.children.right);
    printf(")");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }


    struct lexer *lexer = new_lexer(argv[1]);

    struct ast *ast = NULL;

    if (parse_expression(lexer, &ast))
    {
        print_ast(ast);
        puts("");
    }
    else
        puts("Parsing error");

    ast_free(ast);
    free_lexer(lexer);
}
