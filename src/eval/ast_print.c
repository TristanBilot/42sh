#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "eval/ast_print.h"
#include <err.h>
#include <stdio.h>


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
        puts("end\n");
    else
        puts("Parsing error\n");

    free_lexer(lexer);
}
