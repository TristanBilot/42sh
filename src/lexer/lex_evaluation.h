#ifndef LEX_EVALUATION_H
#define LEX_EVALUATION_H

struct token *lex_great_less_and(const char *c, size_t i);
struct token *lex_io_number(char *c, size_t i);
char *lex_backslash(const char *c, size_t i);
struct token *lex_great_less(char *c, size_t i);
struct token *lex_semicolon_newline(char *c, size_t i);
struct token *lex_assignment_word(char *c, size_t *i);
struct token *lex_assignment_value(char *c, size_t i);

/**
** \brief Return the associated keyword of a string token.
**
** \param c the string to be compared to all the keywords.
*/
enum token_type evaluate_keyword(char *c);

/**
** \brief Return the associated type of a string token.
**
** \param c the string to be compared to all the tokens.
*/
enum token_type evaluate_token(char *c);


#endif /* ! LEX_EVALUATION_H */
