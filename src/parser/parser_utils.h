#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdbool.h>
#include "lexer/token.h"

bool is_redirection(struct token *token);

#endif /* PARSER_UTILS_H */
