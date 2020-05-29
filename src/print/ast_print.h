/**
** \file ast_print.h
** \author Team
** \brief Print functions
** \version 0.1
** \date 2020-05-03
**
** \copyright Copyright (c) 2020
**
*/

#ifndef AST_PRINT_H
#define AST_PRINT_H

#include "../parser/parser.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/string_utils.h"
#include "../ast/ast.h"
#include <stdio.h>
/**
** \brief print node_input
**
** \param ast
** \param f
*/
void print_node_input(struct node_input *ast, FILE *f);

/**
** \brief print node list
**
** \param ast
** \param f
*/
void print_node_list(struct node_list *ast, FILE *f);
/**
** \brief print node_and_or
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_and_or(struct node_and_or *ast, FILE *f, void *node);
/**
** \brief print node pipeline
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_pipeline(struct node_pipeline *ast, FILE *f, void *node);
/**
** \brief print node command
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_command(struct node_command *ast, FILE *f, void *node);
/**
** \brief print note simple command
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_simple_command(struct node_simple_command *ast, FILE *f, void *node);
/**
** \brief print note shell command
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_shell_command(struct node_shell_command *ast, FILE *f, void *node);
/**
** \brief print node funcdec
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_funcdec(struct node_funcdec *ast, FILE *f, void *node);
/**
** \brief print node redirection
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_redirection(struct node_redirection *ast, FILE *f, void *node);
/**
** \brief print node prefix
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_prefix(struct node_prefix *ast, FILE *f, void *node);
/**
** \brief print node element
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_element(struct node_element *ast, FILE *f, void *node);
/**
** \brief print node compound list
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_compound_list(struct node_compound_list *ast, FILE *f, void *node);
/**
** \brief print node while
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_while(struct node_while *ast, FILE *f, void *node);
/**
** \brief print node until
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_until(struct node_until *ast, FILE *f, void *node);
/**
** \brief print node case
**
** \param ast
** \param f
** \param node
** \return * void
*/

void print_node_case(struct node_case *ast, FILE *f, void *node);
/**
** \brief print node if
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_if(struct node_if *ast, FILE *f, void *node);
/**
** \brief print node elif
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_elif(struct node_if *ast, FILE *f, void *node);
/**
** \brief print node for
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_for(struct node_for *ast, FILE *f, void *node);
/**
** \brief print node else clause
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_else_clause(struct node_else_clause *ast, FILE *f, void *node);
/**
** \brief print node do group
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_do_group(struct node_do_group *ast, FILE *f, void *node);
/**
** \brief print node do group
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_case_clause(struct node_case_clause *ast, FILE *f, void *node);
/**
** \brief print node case_item
**
** \param ast
** \param f
** \param node
** \return * void
*/
void print_node_case_item(struct node_case_item *ast, FILE *f, void *node);
/**
** \brief print ast
**
** \param ast
** \return * void
*/
void print_ast(struct node_input *ast);

#endif  /* ! AST_PRINT_H */
