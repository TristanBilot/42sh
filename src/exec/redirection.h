/**
** \file redirection.h
** \author Team
** \brief
** \version 0.1
** \date 2020-05-15
**
** @copyright Copyright (c) 2020
**
*/

#include "../exec/exec.h"

#ifndef REDIRECTION_H
#define REDIRECTION_H

#define TAB_REDI_SIZE 256

//  STRUCTURES

struct file_manager
{
    int save_in;
    int save_out;
    int save_err;
    int fd_to_close;
    FILE *file;
};
//struct file_manager file_manager;

struct std
{
    char *type;
    int ionumber;
    char *file;
};

struct tab_redirection
{
    struct std redirections[TAB_REDI_SIZE];
    int size;
};

//  GLOBAL VARIABLES

struct file_manager *file_manager;


//   FUNCTIONS

/**
** \brief initialize file manager
**
** \return struct file_manager*
*/
struct file_manager *init_file_manager(void);

/**
** \brief create and init the table of redirection
**
** \return struct tab_redirection
*/
struct tab_redirection init_tab_redirection(void);

/**
** \brief
**
** \param tab complete the redirection table with output/input file name
** \param e
** \return struct tab_redirection
*/
struct tab_redirection append_tab_redirection(struct tab_redirection tab, struct node_redirection *e);

/**
** \brief manage duplications for each redirections
**
** \param tab
** \return true
** \return false
*/
bool manage_duplication(struct tab_redirection tab);

/**
** \brief apply file descriptor duplication from file name
**
** \param file
** \param flag
** \param io
** \param ptr_fd
** \return true
** \return false
*/
bool dup_file(char *file, char *flag, int io);

/**
** \brief apply file descriptor duplication from file descriptor
**
** \param out
** \param flag
** \param io
** \return true
** \return false
*/
bool dup_fd(int file, char *flag, int io);

#endif /* ! REDIRECTION_H */

