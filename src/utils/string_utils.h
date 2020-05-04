/**
** \file string_utils.h
** \author Team 
** \brief String usage functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/
#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256

/**
** \brief Return the associated string of a token type.
**
** \param type the enum value of the token.
*/
char *type_to_str(int type);

/**
** \brief Return true is a == b
**
** \param a the first string to be compared.
** \param b the decond string to be compared.
*/
int is(const char *a, const char *b);

/**
** \brief Return true is c is a number.
**
** \param c the caracter.
*/
int is_number(char c);

/**
** \brief Return the substring between pos and len - 1
**
** \param src the string.
** \param pos the starting index.
** \param len the ending index.
*/
char *substr(char *src, int pos, int len);

/**
** \brief Print an error in stderr when an invalid token appeared.
**
** \param msg the message to display.
*/
void error(char *msg);
char *my_strdup(const char *c);

#endif /* ! STRING_H */
