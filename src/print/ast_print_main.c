#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
#include "../garbage_collector/garbage_collector.h"
#include "../storage/program_data_storage.h"
#include "./ast_print.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./ast_print \"expression\"");
        return 1;
    }
    new_garbage_collector();
    new_program_data_storage(argc, argv);
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;

    if ((lexer = new_lexer(argv[1])))
    {
        if ((ast = parse(lexer)))
        {
            // exec_node_input(ast);
            print_ast(ast);
        }
    }

    int ret = atoi(program_data->last_cmd_status);
    free_program_data_storage();
    free_garbage_collector();
    free(garbage_collector);
    return ret;
}
