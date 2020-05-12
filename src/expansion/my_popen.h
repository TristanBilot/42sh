/**
** \file my_popen.h
** \author Team 
** \brief Function for command substitution
** \version 0.1
** \date 2020-05-13
** 
** @copyright Copyright (c) 2020
** 
*/

#ifndef MY_POPEN_H
#define MY_POPEN_H

#include "expansion.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define SET_ERRNO_AND_RETURN(err) \
    errno = err; \
    return NULL;

FILE *my_popen(const char *cmd, const char *mode);
int my_pclose(FILE *stream);

#endif /* MY_POPEN_H */
