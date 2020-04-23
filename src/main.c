#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "./main.h"
#include "./parser/parser.h"
#include "./lexer/lexer.h"
#include "./utils/xalloc.h"
#include "./exec/exec.h"
#include "./utils/string_utils.h"

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
}

void init_42sh_process()
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    print_prompt();
    while ((read = getline(&line, &len, stdin)) != -1)
    {
        //printf("==>%zu", len);
        //line[len - 2] = '\0';
        lexer = new_lexer(line);
        // struct token *token = NULL;
        /*while ((token = pop(lexer)))
            printf("%s %s\n", type_to_str(token->type), token->value);*/

        
        ast = parse(lexer);

        if (exec_node_input(ast))
            printf("Error");

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
    static struct option long_options[] =
    {
        {"c", required_argument, 0, 'c'},
        {"ast-printer", no_argument, 0, 'a'},
        {"norc", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(ac, av, "nac:", long_options, &option_index)) != -1)
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
    init_42sh_process();
}

