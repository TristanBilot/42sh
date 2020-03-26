#ifndef LEXER_H
#define LEXER_H

#include "lexer/token.h"

/**
** \brief Lexer architecture and methods.
**
** \param input the full input string.
** \param token_list the linked-list of tokens.
** \param current the current token pointer in the list.
*/
struct lexer {
    const char *input;
    struct token_list *token_list;
};

/**
** \brief Allocate and init a new lexer.
** \param str the string to use as input stream.
*/
struct lexer *new_lexer(const char *str);

/**
** \brief Free all ressources allocated in the lexer.
**
** \param lexer the lexer to free.
*/
void free_lexer(struct lexer *lexer);

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

void append(struct lexer *lexer, struct token *token);

int is(char *a, char *b);

enum token_type evaluate_token(char *c);

void lexer_init(struct lexer *lexer);

#endif /* ! LEXER_H */
