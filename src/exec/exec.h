/**
** \file exec.h
** \author Team 
** \brief Execution functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

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
#include "../utils/buffer.h"
#include "../utils/string_utils.h"
#include "../utils/my_itoa.h"
#include "../utils/xalloc.h"
#include "../storage/var_storage.h"

#define NB_MAX_PIPE 10
#define ERROR(msg) \
            fprintf(stderr, "%s\n", msg); \
            return true; \

struct commands
{
    const char *name;
    void (*function)(char **args);
};

/** Global for continue command **/
struct command_continue{
    bool is_continue; //= 0;
    int time_to_loop; //= 1;
    bool from_loop;//= false;
    int current_loop;//= O;
};

struct command_continue cont;

/** Global for continue command **/
void init_continue();


/**
** \brief execute input
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_input(struct node_input *ast);

/**
** \brief execute list
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_list(struct node_list *ast);

/**
** \brief execute and/or
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_and_or(struct node_and_or *ast);

/**
** \brief execute pipeline
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_pipeline(struct node_pipeline *ast);

/**
** \brief execute command
** 
** \param ast 
** \param with_fork 
** \return true 
** \return false 
*/
bool exec_node_command(struct node_command *ast, bool with_fork);

/**
** \brief execute simple command
** 
** \param ast 
** \param with_fork 
** \return true 
** \return false 
*/
bool exec_node_simple_command(struct node_simple_command *ast, bool with_fork);

/**
** \brief execute shell command
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_shell_command(struct node_shell_command *ast);

/**
** \brief execute funcdec
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_funcdec(struct node_funcdec *ast);

/**
** \brief execute redirection
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_redirection(struct node_redirection *ast);

/**
** \brief execute prefix
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_prefix(struct node_prefix *ast);

/**
** \brief execute element
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_element(struct node_element *ast);

/**
** \brief execute compound list
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_compound_list(struct node_compound_list *ast);

/**
** \brief execute while
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_while(struct node_while *ast);

/**
** \brief execute until
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_until(struct node_until *ast);

/**
** \brief execute case
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_case(struct node_case *ast);

/**
** \brief execute if
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_if(struct node_if *ast);

/**
** \brief execute elif
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_elif(struct node_if *ast);

/**
** \brief execute for
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_for(struct node_for *ast);

/**
** \brief execute else clause
** 
** \param ast
** \return true 
** \return false 
*/
bool exec_node_else_clause(struct node_else_clause *ast);

/**
** \brief execute do group
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_do_group(struct node_do_group *ast);

/**
** \brief execute case clause
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_case_clause(struct node_case_clause *ast, char *word_to_found);

/**
** \brief execute case item
** 
** \param ast 
** \return true 
** \return false 
*/
bool exec_node_case_item(struct node_case_item *ast, char *word_to_found);
/**
**\brief for function to execute different range
**
**\param r 
**\param ast 
**\return int 
*/
int perform_for_range(struct range *r, struct node_for *ast);
/**
**\brief for function to perform enumeration
**
**\param ast 
**\param len_range 
**\return true 
**\return false 
*/
bool perform_for_enumeration(struct node_for *ast, int len_range);

#endif /* EXEC_H */
