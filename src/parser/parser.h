/**
** \file parser.h
** \author Team 
** \brief Parsing functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"

#include <stdbool.h>
/**
**\brief initialize a parser
**
**\param lexer 
**\return struct parser* 
*/
struct parser *init_parser(struct lexer *lexer);
/**
**\brief look the next token without moving the list of tokens
**
**\param parser 
**\param expected_token 
**\return true 
**\return false 
*/
bool parse_look_ahead(struct parser *parser, struct token *expected_token);
/**
**\brief parse all of the token given by lexer
**
**\param lexer 
**\return void* 
*/
void *parse(struct lexer *lexer);
/**
**\brief parse rule input
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_input(struct parser *parser, struct node_input **ast);
/**
**\brief parse rule list
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_list(struct parser *parser, struct node_list **ast);
/**
**\brief parse rule and or
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_and_or(struct parser *parser, struct node_and_or **ast);
/**
**\brief parse rule pipeline
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_pipeline(struct parser *parser, struct node_pipeline **ast);
/**
**\brief parse rule command
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_command(struct parser *parser, struct node_command **ast);
/**
**\brief parse rule simple command
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_simple_command(struct parser *parser, struct node_simple_command **ast);
/**
**\brief parse rule shell command
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_shell_command(struct parser *parser, struct node_shell_command **ast);
/**
**\brief parse rule funcdec
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_funcdec(struct parser *parser, struct node_funcdec **ast);
/**
**\brief parse rule redirection
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_redirection(struct parser *parser, struct node_redirection **ast);
/**
**\brief parse rule element
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/

bool parse_element(struct parser *parser, struct node_element **ast);
/**
** \brief parse rule prefix
** 
** \param parser 
** \param ast 
** \return true 
** \return false 
*/
bool parse_prefix(struct parser *parser, struct node_prefix **ast);
/**
**\brief parse rule compound list
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_compound_list(struct parser *parser, struct node_compound_list **ast);
/**
**\brief parse rule for
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_rule_for(struct parser *parser, struct node_for **ast);
/**
**\brief parse rule while
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_rule_while(struct parser *parser, struct node_while **ast);
/**
**\brief parse rule until
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_rule_until(struct parser *parser, struct node_until **ast);
/**
**\brief parse rule case
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_rule_case(struct parser *parser, struct node_case **ast);
/**
**\brief parse rule if
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_rule_if(struct parser *parser, struct node_if **ast);
/**
**\brief parse else clause
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_else_clause(struct parser *parser, struct node_else_clause **ast);
/**
**\brief parse rule do group
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_do_group(struct parser *parser, struct node_do_group **ast);
/**
**\brief parse rule case clause
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_case_clause(struct parser *parser, struct node_case_clause **ast);
/**
**\brief parse rule case item
**
**\param parser 
**\param ast 
**\return true 
**\return false 
*/
bool parse_case_item(struct parser *parser, struct node_case_item **ast);
/**
**\brief free the parser
**
**\param p 
*/
void free_parser(struct parser *p);
#endif /* ! PARSER_H */
