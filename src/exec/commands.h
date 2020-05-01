#ifndef COMMANDS_H_
#define COMMANDS_H_


void echo(char **args);
void cd(char **args);
void export(char **args);
void exit_shell(char **args);

struct echo_tab
{
  char name;
  char corresp;
};

#endif /* COMMANDS_H_ */