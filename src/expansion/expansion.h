/**
** \file expansion.h
** \author Team 
** \brief Var storage structures and functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef EXPANSION_H
#define EXPANSION_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct program_data_storage *program_data;

char *substitute(char *word);

/* ++++++++++++++ Parameters ++++++++++++++ */

struct program_data_storage
{
    /* argc & argv aren't with the [0] = binary_name */
    char *binary_name;     // $0
    char **argv;           // $* $@
    int argc;              // $#
    char *last_cmd_status; // $?
};

enum param_type
{
    PAR_NUMBER,
    PAR_STAR,
    PAR_AT,
    PAR_HASH,
    PAR_QUES,
    PAR_UNKNOWN
};

void new_program_data_storage(int argc, char *argv[]);
void free_program_data_storage(void);
void update_last_status(int status);
char *perform_var_expansion(char *word);
enum param_type is_special_char(char c);

char *substitute_number(char c);
struct buffer *substitute_star(void);
struct buffer *substitute_at(void);
char *substitute_hash(void);
char *substitute_ques(void);

char *substitute_random(char *word, size_t *i, bool *should_continue, int is_brack);
char *substitute_uid(char *word, size_t *i, bool *should_continue, int is_brack);
char *substitute_oldpwd(char *word, size_t *i, bool *should_continue, int is_brack);
char *substitute_ifs(char *word, size_t *i, bool *should_continue, int is_brack);

enum param_type is_special_char(char c);
int get_random_int(void);
size_t get_next_brack_index(const char *c, size_t j);
size_t get_next_dollar_index(const char *c, size_t j);

/* ++++++++++++++ Tilde expansion ++++++++++++++ */

char *perform_tilde_expansion(char *word);
char *substitute_minus_tilde(char *word, size_t *i);
char *substitute_plus_tilde(char *word, size_t *i);
char *substitute_tilde(char *word, size_t *i);

#endif /* EXPANSION_H */
