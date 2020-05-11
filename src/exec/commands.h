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
** \brief function to give a file to the 42sh
**
** \param args 
*/
void load_file(char *path);
/**
** \brief implementation of command sourcefnac
**
** \param args 
*/
void source(char **args);
/**
** \brief implementation of command echo
**
** \param args 
*/
void echo(char **args);
/**
** \brief implementation of command cd
**
** \param args 
*/
void cd(char **args);

/**
** \brief implementation of command export
**
** \param args 
*/
void export(char **args);
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
int	print_without_sp(char *c);
void print_echo(char **args, bool e, bool n);
#endif /* COMMANDS_H_ */