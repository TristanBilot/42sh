#ifndef STRING_H
#define STRING_H

/**
** \brief Return the associated string of a token type.
**
** \param type the enum value of the token.
*/
char *type_to_str(int type);

/**
** \brief Empty a string buffer.
**
** \param buffer the string to be clear.
** \param size the length of the buffer.
*/
void flush(char *buffer, int size);

/**
** \brief Return true is a == b
**
** \param a the first string to be compared.
** \param b the decond string to be compared.
*/
int is(char *a, char *b);

#endif /* ! STRING_H */
