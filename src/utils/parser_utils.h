#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdbool.h>

bool is_redirection(struct token *token);

struct node_prefix *append_prefix(struct node_simple_command *ast, struct node_prefix *prefix);
struct node_element *append_element(struct node_simple_command *ast, struct node_element *element);

struct node_redirection *append_redirection(
    struct node_command *ast,
    struct node_redirection *redirection); 

struct range *append_value_to_for(
    struct node_for *ast,
    char *value) ;

struct word_list *append_word_list(struct node_case_item *ast, char *value);
enum shell_type get_shell_command_type(int type);

#endif /* PARSER_UTILS_H */
