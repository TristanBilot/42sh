/**
** \file lexer.h
** \author Team
** \brief Main lexing functions
** \version 0.1
** \date 2020-05-03
**
** \copyright Copyright (c) 2020
**
*/

#ifndef LEXER_H
#define LEXER_H

#include "../lexer/token.h"
#include <stdbool.h>

/**
** \brief Lexer architecture and methods.
**
** \param input the full input string.
** \param token_list the linked-list of tokens.
*/
struct lexer {
    char *input;
    struct token_list *token_list;
};

/**
** \brief Allocate and init a new lexer.
** \param str the string to use as input stream.
*/
struct lexer *new_lexer(char *str);

/**
** \brief Return the next token without consume it.
**
** \return the next token from the input stream
** \param lexer the lexer to lex from
*/
struct token *peek(struct lexer *lexer);

/**
** \brief Return and consume the next token from the input stream.
**
** \return the next token from the input stream
** \param lexer the lexer to lex from
*/
struct token *pop(struct lexer *lexer);

/**
** \brief Append a new token to the token_list of the lexer.
**
** \param lexer the lexer.
** \param token the token to append.
*/
void append(struct lexer *lexer, struct token *token);

/**
** \brief Fill the token list by creating all the tokens from
** the given string.
**
** \param lexer the lexer.
*/
bool init_lexer(struct lexer *lexer);

int is_separator(char c);

#endif /* ! LEXER_H */
