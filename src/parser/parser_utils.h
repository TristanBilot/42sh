/**
** \file parser_utils.h
** \author Team
** \brief Additional functions needed by parser
** \version 0.1
** \date 2020-05-03
**
** \copyright Copyright (c) 2020
**
*/

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
