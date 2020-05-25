#ifndef ARITHMETIC_PARSER_H
#define ARITHMETIC_PARSER_H

#include "../ast/arithmetic_ast.h"
#include "../lexer/arithmetic_lexer.h"

#include <stdbool.h>

/**
** \brief Parse from the given lexer and allocate a new ast in `*ast`
**
** \return true if no error occured, false otherwise.
** \param lexer lexer to get token from
** \param ast placeholder for the ast to build
*/

bool parse_parenthesis(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_texp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_log_not(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_exponent(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_sexp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_exp(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_bit_and(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_bit_xor(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_bit_or(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_log_and(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_log_or(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);
bool parse_expression(struct arithmetic_lexer *lexer, struct arithmetic_ast **ast);

#endif /* ! ARITHMETIC_PARSER_H */
