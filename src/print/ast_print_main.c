#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
#include "../garbage_collector/garbage_collector.h"
#include "../eval/ast_print.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }
    new_garbage_collector();
    struct lexer *lexer = new_lexer(argv[1]);
    struct node_input *ast = NULL;

    if ((ast = parse(lexer)))
    {
        // exec_node_input(ast);
        print_ast(ast);
    }
    free_garbage_collector();
    free(garbage_collector);
}
