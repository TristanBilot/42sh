/**
** \file free.h
** \author Team
** \brief Free functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef FREE_H
#define FREE_H

#include "../ast/ast.h"

/**
** \brief 
**
** \param ast 
*/
void free_input(struct node_input *ast);

/**
** \brief free list node
** 
** \param ast 
*/
void free_list(struct node_list *ast);

/**
** \brief free and/or node
** 
** \param ast 
*/
void free_and_or(struct node_and_or *ast);

/**
** \brief free pipeline node
** 
** \param ast 
*/
void free_pipeline(struct node_pipeline *ast);

/**
** \brief 
** 
** \param ast 
*/
void free_command(struct node_command *ast);

/**
** \brief free simple command node
** 
** \param ast 
*/
void free_simple_command(struct node_simple_command *ast);

/**
** \brief free shell command node
** 
** \param ast 
*/
void free_shell_command(struct node_shell_command *ast);

/**
** \brief free funcdec node
** 
** \param ast 
*/
void free_funcdec(struct node_funcdec *ast);

/**
** \brief free redirection node
** 
** \param ast 
*/
void free_redirection(struct node_redirection *ast);

/**
** \brief free prefix node
** 
** \param ast 
*/
void free_prefix(struct node_prefix *ast);

/**
** \brief free element node
** 
** \param ast 
*/
void free_element(struct node_element *ast);

/**
** \brief free compound list node
** 
** \param ast 
*/
void free_compound_list(struct node_compound_list *ast);

/**
** \brief free while node
** 
** \param ast 
*/
void free_while(struct node_while *ast);

/**
** \brief free until node
** 
** \param ast 
*/
void free_until(struct node_until *ast);

/**
** \brief free case node
** 
** \param ast 
*/
void free_case(struct node_case *ast);

/**
** \brief free if node
** 
** \param ast 
*/
void free_if(struct node_if *ast);

/**
** \brief free for node
** 
** \param ast 
*/
void free_for(struct node_for *ast);

/**
** \brief free else clause node
** 
** \param ast 
*/
void free_else_clause(struct node_else_clause *ast);

/**
** \brief free do group node
** 
** \param ast 
*/
void free_do_group(struct node_do_group *ast);

/**
** \brief free case clause
** 
** \param ast 
*/
void free_case_clause(struct node_case_clause *ast);

/**
** \brief free case item node
** 
** \param ast 
*/
void free_case_item(struct node_case_item *ast);



#endif/* FREE_H */
