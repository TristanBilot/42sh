#ifndef LEX_EVALUATION_H
#define LEX_EVALUATION_H

char *lex_backslash(char *c, int i);

/**
** \brief Return the associated type of a string token.
**
** \param c the string to be compared to all the tokens.
*/
enum token_type evaluate_token(char *c);

#endif /* ! LEX_EVALUATION_H */
