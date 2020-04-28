#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"

#include <stdbool.h>

struct parser *init_parser(struct lexer *lexer);

bool parse_look_ahead(struct parser *parser, struct token *expected_token);

void *parse(struct parser *parser, struct lexer *lexer);

bool parse_input(struct parser *parser, struct node_input **ast);

bool parse_list(struct parser *parser, struct node_list **ast);

bool parse_and_or(struct parser *parser, struct node_and_or **ast);

bool parse_pipeline(struct parser *parser, struct node_pipeline **ast);

bool parse_command(struct parser *parser, struct node_command **ast);

bool parse_simple_command(struct parser *parser, struct node_simple_command **ast);

bool parse_shell_command(struct parser *parser, struct node_shell_command **ast);

bool parse_funcdec(struct parser *parser, struct node_funcdec **ast);

bool parse_redirection(struct parser *parser, struct node_redirection **ast);

bool parse_prefix(struct parser *parser, struct node_prefix **ast);

bool parse_element(struct parser *parser, struct node_element **ast);

bool parse_compound_list(struct parser *parser, struct node_compound_list **ast);

bool parse_rule_for(struct parser *parser, struct node_for **ast);

bool parse_rule_while(struct parser *parser, struct node_while **ast);

bool parse_rule_until(struct parser *parser, struct node_until **ast);

bool parse_rule_case(struct parser *parser, struct node_case **ast);

bool parse_rule_if(struct parser *parser, struct node_if **ast);

bool parse_else_clause(struct parser *parser, struct node_else_clause **ast);

bool parse_do_group(struct parser *parser, struct node_do_group **ast);

bool parse_case_clause(struct parser *parser, struct node_case_clause **ast);

bool parse_case_item(struct parser *parser, struct node_case_item **ast);

void free_parser(struct parser *p);
#endif /* ! PARSER_H */
