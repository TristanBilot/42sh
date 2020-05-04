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

#endif /* COMMANDS_H_ */