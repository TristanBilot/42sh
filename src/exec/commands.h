/**
** \file commands.h
** \author Team
** \brief Extra commands functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_


/**
** \brief implementation of command echo
**
** \param args 
*/
void echo(char **args, int *ptr_fd);
/**
** \brief implementation of command cd
**
** \param args 
*/
void cd(char **args, int *ptr_fd);

/**
** \brief implementation of command export
**
** \param args 
*/
void export(char **args, int *ptr_fd);
/**
**\brief implementation of exit_shell
**
**\param args 
*/
void exit_shell(char **args);

struct echo_tab
{
  char name;
  char corresp;
};

void print_args(char **args);
int	print_without_sp(char *c, int *ptr_fd);
void print_echo(char **args, bool e, bool n, int *ptr_fd);
#endif /* COMMANDS_H_ */