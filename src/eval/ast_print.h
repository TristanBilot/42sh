#include "parser/parser.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/string.h"
#include "ast/ast.h"


void print_node_input(struct node_input *ast);
void print_node_list(struct node_list *ast);
void print_node_and_or(struct node_and_or *ast);
void print_node_pipeline(struct node_pipeline *ast);
void print_node_command(struct node_command *ast);
void print_node_simple_command(struct node_simple_command *ast);
void print_node_shell_command(struct node_shell_command *ast);
void print_node_funcdec(struct node_funcdec *ast);
void print_node_redirection(struct node_redirection *ast);
void print_node_prefix(struct node_prefix *ast);
void print_node_element(struct node_element *ast);
void print_node_compound_list(struct node_compound_list *ast);
void print_node_while(struct node_while *ast);
void print_node_until(struct node_until *ast);
void print_node_case(struct node_case *ast);
void print_node_if(struct node_if *ast);
void print_node_for(struct node_for *ast);
void print_node_else_clause(struct node_else_clause *ast);
void print_node_do_group(struct node_do_group *ast);
void print_node_case_clause(struct node_case_clause *ast);
void print_node_case_item(struct node_case_item *ast);