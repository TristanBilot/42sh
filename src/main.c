#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "./main.h"
#include "./parser/parser.h"
#include "./lexer/lexer.h"
#include "./ast/free.h"
#include "./utils/xalloc.h"
#include "./exec/exec.h"
#include "./utils/string_utils.h"
#include "./eval/ast_print.h"
#include "./var_storage/var_storage.h"
#include "./expansion/expansion.h"

static struct option long_options[] =
{
    {"c", required_argument, 0, 'c'},
    {"ast-printer", no_argument, 0, 'a'},
    {"norc", no_argument, 0, 'n'},
    {0, 0, 0, 0}
};

void print_usage()
{
    fprintf(stdout, USAGE);
    exit(1);
}

void print_prompt()
{
    char *buff = NULL;
    if ((buff = getcwd(NULL, 0)) == NULL)
        exit(1);
    if (isatty(0) == 1)
    {
        dprintf(0, "%s[%s%s%s", START_COLOR, CYAN, buff, END_COLOR);
        dprintf(0, "%s[%s %s> %s", START_COLOR, CYAN, BLINK, END_COLOR);
    }
    free(buff);
    buff = NULL;
}

static void init_42sh_process(struct option_sh *option)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    struct lexer *lexer = NULL;
    struct parser *parser = NULL;
    struct node_input *ast = NULL;
    if (option->cmd)
    {
        lexer = new_lexer(option->cmd);
        ast = parse(parser, lexer);
        if (exec_node_input(ast))
            printf("Error affiché dans le main");
        free_parser(parser);
    }
    print_prompt();
    while ((read = getline(&line, &len, stdin)) != -1)
    {
        lexer = new_lexer(line);
        ast = parse(parser, lexer);

        if (exec_node_input(ast))
            printf("Error on exec ast.");
        if (option->print_ast_flag)
            print_ast(ast);
        free_parser(parser);
        free_input(ast);
        print_prompt();
    }
    if (line)
        free(line);
}

static struct option_sh *init_option_sh()
{
    struct option_sh *option = xmalloc(sizeof(struct option_sh));
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
    new_var_storage();
    new_program_data_storage(ac, av);
    srand(time(NULL));
    
    while ((opt = getopt_long(ac, av, "nac:", long_options, &option_index)) != -1)
    {
        if (opt == 'n')
            option->norc_flag = true;
        else if (opt == 'a')
            option->print_ast_flag = true;
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
    init_42sh_process(option);
    free_var_storage();
    free_program_data_storage();
}
