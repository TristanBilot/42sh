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
** \brief function to delete an alias
**
** \param args 
*/
void delete_alias(char **args);
/**
** \brief function to create an alias
**
** \param args 
*/
void create_alias(char **args);
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
** \brief 
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
**
*/
void exit_shell(void);

struct echo_tab
{
  char name;
  char corresp;
};
/**
**\brief Print all args on stdout
**
**\param args 
*/
void print_args(char **args);
/**
**\brief Particular print with option -e from echo
**
**\param c 
**\return int 
*/
int	print_without_sp(char *c);
/**
**\brief Echo function 
**
**\param args 
**\param e 
**\param n 
*/
void print_echo(char **args, bool e, bool n);


void func_continue(char **args);

#endif /* COMMANDS_H_ */