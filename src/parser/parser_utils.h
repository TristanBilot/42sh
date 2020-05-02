#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdbool.h>
#include "../lexer/token.h"
/**
**\brief check if there is a redirection
**
**\param token 
**\return true 
**\return false 
*/
bool is_redirection(struct token *token);

#endif /* PARSER_UTILS_H */
