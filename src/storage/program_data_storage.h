/**
** \file token.h
** \author Team 
** \brief Token structures and functions
** \version 0.1
** \date 2020-05-06
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef PROGRAM_DATA_STORAGE_H
#define PROGRAM_DATA_STORAGE_H

struct program_data_storage *program_data;
struct env_storage *env_storage;

struct program_data_storage
{
    /* argc & argv aren't with the [0] = binary_name */
    char *binary_name;     // $0
    char **argv;           // $* $@
    int argc;              // $#
    char *last_cmd_status; // $?
};

struct env_storage
{
    char *old_pwd;
    // struct variable **env_variables;
};

void new_program_data_storage(int argc, char *argv[]);
void append_program_data(char *element);
void free_program_data_storage(void);
void update_last_status(int status);

#endif /* PROGRAM_DATA_STORAGE_H */
