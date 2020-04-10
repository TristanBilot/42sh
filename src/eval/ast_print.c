#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include <err.h>
#include <stdio.h>

static void print_ast(struct node_input *ast)
{
    if (!ast)
        return;

}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }

    struct lexer *lexer = new_lexer(argv[1]);
    struct node_input *ast = NULL;

    if ((ast = parse(lexer)))
        print_ast(ast);
    // else
    //     puts("Parsing error\n");

    free_lexer(lexer);
}
