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

#define _XOPEN_SOURCE 700
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char *substitute(char *word);

/* ++++++++++++++ Parameters ++++++++++++++ */

enum param_type
{
    PAR_NUMBER,
    PAR_STAR,
    PAR_AT,
    PAR_HASH,
    PAR_QUES,
    PAR_UNKNOWN
};

char *perform_var_expansion(char *word);
enum param_type is_special_char(char c);

char *substitute_number(char c);
struct buffer *substitute_star(void);
struct buffer *substitute_at(void);
char *substitute_hash(void);
char *substitute_ques(void);

char *substitute_random(char *word, size_t *i, bool *should_continue, int is_brack);
char *substitute_uid(char *word, size_t *i, bool *should_continue, int is_brack);
char *substitute_pid(char *word, size_t *i, bool *should_continue, int is_brack);
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


/* ++++++++++++++ Command substitution ++++++++++++++ */

char *perform_command_substitution(char *word);

/* ++++++++++++++ Arithmetic substitution ++++++++++++++ */

char *perform_arithmetic_substitution(char *word);

#endif /* EXPANSION_H */
