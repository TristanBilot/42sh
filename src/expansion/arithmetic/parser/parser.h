#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"

#include <stdbool.h>

/**
** \brief Parse from the given lexer and allocate a new ast in `*ast`
**
** \return true if no error occured, false otherwise.
** \param lexer lexer to get token from
** \param ast placeholder for the ast to build
*/
bool parse_expression(struct lexer *lexer, struct ast **ast);

#endif /* ! PARSER_H */
