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

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_HISTORY_FILE_NAME "history"
#define HISTORY_MAX 2000

struct history
{
    char **commands;
    int nb_commands;
    int index;
};

struct history *open_history(void);
void append_history_command(struct history *history, char *cmd);
void write_next_history(struct history *history);
void write_prev_history(struct history *history);

void flush_stdin(void);
void write_stdin(char *cmd);
char *get_next_history(struct history *history);
char *get_prev_history(struct history *history);
void load_history(struct history *history);
void free_history(struct history *history);

#endif /* HISTORY_H */
