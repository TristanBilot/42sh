#ifndef LEX_EVALUATION_H
#define LEX_EVALUATION_H

struct token *lex_great_less_and(const char *c, size_t i);
struct token *lex_io_number(char *c, size_t *i);
char *lex_backslash(const char *c, size_t i);
struct token *lex_great_less(char *c, size_t i);
struct token *lex_comments(char *c, size_t i);
struct token *lex_uni_character(char *c, size_t i);
struct token *lex_assignment_word(char *c, size_t *i);
struct token *lex_assignment_value(char *c, size_t *i);
size_t get_previous_separator_index(const char *c, size_t j);

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
