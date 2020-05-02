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