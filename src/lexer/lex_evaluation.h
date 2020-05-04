/**
** \file lex_evaluation.h
** \author Team 
** \brief Unit lexing functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef LEX_EVALUATION_H
#define LEX_EVALUATION_H

/**
**\brief process great less and into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_great_less_and(const char *c, size_t i);
/**
**\brief process io number into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_io_number(char *c, size_t *i);
/**
**\brief process backslash in the lexer
**
**\param c 
**\param i 
**\return char* 
*/
char *lex_backslash(const char *c, size_t i);
/**
**\brief process great less into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_great_less(char *c, size_t i);
/**
**\brief process comments into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_comments(char *c, size_t i);
/**
**\brief process uni character into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_uni_character(char *c, size_t i);
/**
**\brief process assignment word into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_assignment_word(char *c, size_t *i);
/**
**\brief process assignment word into token
**
**\param c 
**\param i 
**\return struct token* 
*/
struct token *lex_assignment_value(char *c, size_t *i);
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
