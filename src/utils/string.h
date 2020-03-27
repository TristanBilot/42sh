#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

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
** \brief Return the substring between pos and len - 1
**
** \param src the string.
** \param pos the starting index.
** \param len the ending index.
*/
char *substr(char *src, int pos, int len);

#endif /* ! STRING_H */