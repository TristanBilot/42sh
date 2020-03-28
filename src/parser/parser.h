#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"

#include <stdbool.h>

struct parser
{

};

bool parse_expression(struct lexer *lexer, struct ast **ast);

#endif /* ! PARSER_H */
