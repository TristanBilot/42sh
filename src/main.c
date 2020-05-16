#include "./main.h"

struct option_sh *init_option_sh()
{
    struct option_sh *option = malloc(sizeof(struct option_sh));
    option->norc_flag = false;
    option->print_ast_flag = false;
    option->cmd = NULL;
    option->file_path = NULL;
    option->shotp = false;
    return option;
}

int main(int ac, char **av)
{
    struct option long_options[] =
    {
        {"c", required_argument, 0, 'c'},
        {"ast-printer", no_argument, 0, 'a'},
        {"norc", no_argument, 0, 'n'},
        {"O", no_argument, 0, 'o'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    int opt = -1;
    new_garbage_collector();
    option = init_option_sh();
    var_storage = new_var_storage();
    alias_storage = new_var_storage();
    new_program_data_storage(ac, av);
    srand(time(NULL));
    
    while ((opt = getopt_long(ac, av, "naoc:", long_options, &option_index)) != -1)
    {
        if (opt == 'n')
            option->norc_flag = true;
        else if (opt == 'a')
            option->print_ast_flag = true;
        else if (opt == 'o')
            option->shotp = true;
        else if (opt == 'c')
            option->cmd = optarg;
        else if (opt == '?')
        {
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            print_usage();
        }
        else
            print_usage();
    }
    if (av[optind])
        option->file_path = av[optind];
    init_42sh_with_history(option);
    int ret = atoi(program_data->last_cmd_status);
    free_garbage_collector();
    free(option);
    free_var_storage(var_storage);
    free_var_storage(alias_storage);
    free_program_data_storage();
    free(garbage_collector);
    return ret;
}
