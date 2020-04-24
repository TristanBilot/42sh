#include <stdio.h>
#include "../parser/parser.h"

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
    {
        exec_node_input(ast);
        print_ast(ast);
    }
    free_lexer(lexer);
}
