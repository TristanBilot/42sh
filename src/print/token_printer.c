#include <stdio.h>
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/string_utils.h"
#include "../garbage_collector/garbage_collector.h"
#include "../storage/program_data_storage.h"
#include "../storage/var_storage.h"
#include "../exec/redirection.h"

struct var_storage *alias_storage;
struct var_storage *var_storage;
struct file_manager *file_manager;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./token_printer \"expression\"");
        return 1;
    }
    new_garbage_collector();
    new_program_data_storage(argc, argv);
    file_manager = init_file_manager();
    var_storage = new_var_storage();
    alias_storage = new_var_storage();
    struct lexer *lexer = NULL;
    if ((lexer = new_lexer(argv[1])))
    {
        struct token *token = NULL;
        while ((token = pop(lexer)))
            printf("%s %s\n", type_to_str(token->type), token->value);
    }

    int ret = atoi(program_data->last_cmd_status);
    free_var_storage(alias_storage);
    free_program_data_storage();
    free_garbage_collector();
    free(garbage_collector);
    return ret;
}
