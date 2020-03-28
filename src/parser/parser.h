#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"

#include <stdbool.h>

static bool parse_look_ahead(struct lexer *lexer, struct token *expected_token);

struct ast_node *parser(struct lexer *lexer);

static bool parse_input(struct lexer *lexer, struct ast_node *ast);

static bool parse_list(struct lexer *lexer, struct ast_node *ast);

static bool parse_and_or(struct lexer *lexer, struct ast_node *ast);

static bool parse_pipeline(struct lexer *lexer, struct ast_node *ast);

static bool parse_command(struct lexer *lexer, struct ast_node *ast);

static bool parse_simple_command(struct lexer *lexer, struct ast_node *ast);

static bool parse_shell_command(struct lexer *lexer, struct ast_node *ast);

static bool parse_funcdec(struct lexer *lexer, struct ast_node *ast);

static bool parse_redirection(struct lexer *lexer, struct ast_node *ast);

static bool parse_prefix(struct lexer *lexer, struct ast_node *ast);

static bool parse_element(struct lexer *lexer, struct ast_node *ast);

static bool parse_coumpound_list(struct lexer *lexer, struct ast_node *ast);

static bool parse_rule_for(struct lexer *lexer, struct ast_node *ast);

static bool parse_rule_while(struct lexer *lexer, struct ast_node *ast);

static bool parse_rule_until(struct lexer *lexer, struct ast_node *ast);

static bool parse_rule_case(struct lexer *lexer, struct ast_node *ast);

static bool parse_rule_if(struct lexer *lexer, struct ast_node *ast);

static bool parse_else_clause(struct lexer *lexer, struct ast_node *ast);

static bool parse_do_group(struct lexer *lexer, struct ast_node *ast);

static bool parse_case_clause(struct lexer *lexer, struct ast_node *ast);

static bool parse_case_item(struct lexer *lexer, struct ast_node *ast);


#endif /* ! PARSER_H */
