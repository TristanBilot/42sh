#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
#include "../garbage_collector/garbage_collector.h"
#include "../storage/program_data_storage.h"
#include "../storage/var_storage.h"
#include "./ast_print.h"

struct var_storage *alias_storage;
struct var_storage *var_storage;
struct file_manager *file_manager;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./ast_print \"expression\"");
        return 1;
    }
    new_garbage_collector();
    new_program_data_storage(argc, argv);
    alias_storage = new_var_storage();
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;

    if ((lexer = new_lexer(argv[1])))
    {
        if ((ast = parse(lexer)))
            print_ast(ast);
    }

    int ret = atoi(program_data->last_cmd_status);
    free_var_storage(alias_storage);
    free_program_data_storage();
    free_garbage_collector();
    free(garbage_collector);
    return ret;
}
