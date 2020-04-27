#ifndef EXEC_H
#define EXEC_H

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../parser/parser.h"
#include "../exec/commands.h"

#define NB_MAX_PIPE 10
#define ERROR(msg) \
            fprintf(stderr, "%s\n", msg); \
            return true; \

struct commands
{
    const char *name;
    void (*function)(char **args);
};

struct std 
{
    char *in;
    char *out;
    char *err;
};

struct tab_redi
{
    struct std dless;
    struct std lessgreat;
    struct std lessand;
    struct std less;
    struct std dgreat;
    struct std greatand;
    struct std clobber;
    struct std great;
    struct std dlessdash;
};


struct tab_redirection *init_tab_redirection(void);
bool exec_node_input(struct node_input *ast);
bool exec_node_list(struct node_list *ast);
bool exec_node_and_or(struct node_and_or *ast);
bool exec_node_pipeline(struct node_pipeline *ast);
bool exec_node_command(struct node_command *ast, bool with_fork);
bool exec_node_simple_command(struct node_simple_command *ast, bool with_fork);
bool exec_node_shell_command(struct node_shell_command *ast);
bool exec_node_funcdec(struct node_funcdec *ast);
bool exec_node_redirection(struct node_redirection *ast);
bool exec_node_prefix(struct node_prefix *ast);
bool exec_node_element(struct node_element *ast);
bool exec_node_compound_list(struct node_compound_list *ast);
bool exec_node_while(struct node_while *ast);
bool exec_node_until(struct node_until *ast);
bool exec_node_case(struct node_case *ast);
bool exec_node_if(struct node_if *ast);
bool exec_node_elif(struct node_if *ast);
bool exec_node_for(struct node_for *ast);
bool exec_node_else_clause(struct node_else_clause *ast);
bool exec_node_do_group(struct node_do_group *ast);
bool exec_node_case_clause(struct node_case_clause *ast);
bool exec_node_case_item(struct node_case_item *ast);

#endif /* EXEC_H */
