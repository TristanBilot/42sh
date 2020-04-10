#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdbool.h>

bool is_redirection(struct token *token);

struct node_prefix *append_prefix(struct node_simple_command *ast, struct node_prefix *prefix);
struct node_element *append_element(struct node_simple_command *ast, struct node_element *element);
struct node_redirection *append_redirection(
    struct node_command *ast,
    struct node_redirection *redirection); 

#endif /* PARSER_UTILS_H */
