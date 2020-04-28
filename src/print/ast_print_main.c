#include <stdio.h>
#include "../parser/parser.h"
#include "../eval/ast_print.h"
#include "../exec/exec.h"
#include "../var_storage/var_storage.h"
#include "../expansion/expansion.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }
    new_var_storage();
    new_program_data_storage(argc, argv);

    struct lexer *lexer = new_lexer(argv[1]);
    struct node_input *ast = NULL;
    struct parser *parser = NULL;
    if ((ast = parse(parser, lexer)))
    {
        exec_node_input(ast);
        print_ast(ast);
    }
    free_parser(parser);
    free_var_storage();
    free_program_data_storage();
}
