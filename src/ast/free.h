#ifndef FREE_H
#define FREE_H

#include "ast/ast.h"

void free_input(struct node_input *ast);
void free_list(struct node_list *ast);
void free_and_or(struct node_and_or *ast);
void free_pipeline(struct node_pipeline *ast);
void free_command(struct node_command *ast);
void free_simple_command(struct node_simple_command *ast);
void free_shell_command(struct node_shell_command *ast);
void free_funcdec(struct node_funcdec *ast);
void free_redirection(struct node_redirection *ast);
void free_prefix(struct node_prefix *ast);
void free_element(struct node_element *ast);
void free_compound_list(struct node_compound_list *ast);
void free_while(struct node_while *ast);
void free_until(struct node_until *ast);
void free_case(struct node_case *ast);
void free_if(struct node_if *ast);
void free_for(struct node_for *ast);
void free_else_clause(struct node_else_clause *ast);
void free_do_group(struct node_do_group *ast);
void free_case_clause(struct node_case_clause *ast);
void free_case_item(struct node_case_item *ast);



#endif/* FREE_H */
