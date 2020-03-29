#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "my_42sh.h"

void print_usage()
{
    fprintf(stderr, USAGE);
    exit(1);
}

static struct option_sh *init_option_sh()
{
    struct option_sh *option = malloc(sizeof(struct option_sh));
    if (option == NULL)
        err(2, strerror(errno));
    option->norc_flag = false;
    option->print_ast_flag = false;
    option->cmd = NULL;
    return option;
}

int main(int ac, char **av)
{
    int option_index = 0;
    int opt = -1;
    struct option_sh *option = init_option_sh();
    static struct option long_options[] =
    {
        {"c", required_argument, 0, 'c'},
        {"ast-printer", no_argument, 0, 'a'},
        {"norc", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(ac, av, "nac:", long_options, &option_index)) == -1)
    {
        if (opt == 'n')
            option->norc_flag = true;
        else if (opt == 'a')
            option->print_ast_flag = true;
        else if (opt == 'c')
            option->cmd = optarg;
        else if (opt == '?') {
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
}