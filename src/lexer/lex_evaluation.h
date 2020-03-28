#ifndef LEX_EVALUATION_H
#define LEX_EVALUATION_H

struct token *lex_great_less_and(const char *c, int i);
struct token *lex_io_number(char *c, int i);
char *lex_backslash(const char *c, int i);
struct token *lex_great_less(char *c, int i);

/**
** \brief Return the associated type of a string token.
**
** \param c the string to be compared to all the tokens.
*/
enum token_type evaluate_token(char *c);

#endif /* ! LEX_EVALUATION_H */
