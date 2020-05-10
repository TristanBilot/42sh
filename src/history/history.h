/**
** \file history.h
** \author Team 
** \brief History functions
** \version 0.1
** \date 2020-05-04
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef HISTORY_H
#define HISTORY_H

// #define _XOPEN_SOURCE 700

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#define MIN(x,y) return x < y ? x : y

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>
#include <unistd.h>

#define DEFAULT_HISTORY_FILE_NAME "history"
#define HISTORY_MAX 2000

struct history
{
    char **commands;
    int nb_commands;
    int index;
    int nb_lines;
};

struct history *open_history(void);
void append_history_command(struct history *history, char *cmd);
char *write_next_history(struct history *history);
char *write_prev_history(struct history *history);

void flush_stdin(void);
void write_stdin(char *cmd);
char *get_next_history(struct history *history);
char *get_prev_history(struct history *history);
void load_history(struct history *history);
void free_history(struct history *history);

void reset_terminal_mode();
void set_conio_terminal_mode();
int kbhit();
int getch();
bool is_only_spaces(char *cmd);

#endif /* HISTORY_H */
