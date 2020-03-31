#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

#include <stdbool.h>

struct parser
{
    struct lexer *lexer;
    struct token *current_token;
};

struct parser *init_parser(struct lexer *lexer);

bool parse_look_ahead(struct parser *parser, struct token *expected_token);

void *parse(struct lexer *lexer);

bool parse_input(struct parser *parser, void *ast);

bool parse_list(struct parser *parser, void *ast);

bool parse_and_or(struct parser *parser, void *ast);

bool parse_pipeline(struct parser *parser, void *ast);

bool parse_command(struct parser *parser, void *ast);

bool parse_simple_command(struct parser *parser, void *ast);

bool parse_shell_command(struct parser *parser, void *ast);

bool parse_funcdec(struct parser *parser, void *ast);

bool parse_redirection(struct parser *parser, void *ast);

bool parse_prefix(struct parser *parser, void *ast);

bool parse_element(struct parser *parser, void *ast);

bool parse_compound_list(struct parser *parser, void *ast);

bool parse_rule_for(struct parser *parser, void *ast);

bool parse_rule_while(struct parser *parser, void *ast);

bool parse_rule_until(struct parser *parser, void *ast);

bool parse_rule_case(struct parser *parser, void *ast);

bool parse_rule_if(struct parser *parser, void *ast);

bool parse_else_clause(struct parser *parser, void *ast);

bool parse_do_group(struct parser *parser, void *ast);

bool parse_case_clause(struct parser *parser, void *ast);

bool parse_case_item(struct parser *parser, void *ast);


#endif /* ! PARSER_H */
