#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdbool.h>

/**
** \brief Return true if the token is a redirection.
** 
** \param token 
** \return true 
** \return false 
*/
bool is_redirection(struct token *token);

/**
** \brief Add prefix node to the prefix list of simple command node.
** 
** \param ast 
** \param prefix 
** \return struct node_prefix* 
*/
struct node_prefix *append_prefix(struct node_simple_command *ast, struct node_prefix *prefix);
/**
** \brief Add element node to the element list of the simple command node.
** 
** \param ast 
** \param element 
** \return struct node_element* 
*/
struct node_element *append_element(struct node_simple_command *ast, struct node_element *element);
/**
** \brief Add redirection node to the redirection list of the command node.
** 
** \param ast 
** \param redirection 
** \return struct node_redirection* 
*/
struct node_redirection *append_redirection(
    struct node_command *ast,
    struct node_redirection *redirection); 
/**
** \brief Add new value to the range list of the for node.
** 
** \param ast 
** \param value 
** \return struct range* 
*/
struct range *append_value_to_for(
    struct node_for *ast,
    char *value) ;
/**
** \brief Add new value to the pipeline list of the case item node.
** 
** \param ast 
** \param value 
** \return struct word_list* 
*/
struct word_list *append_word_list(struct node_case_item *ast, char *value);
/**
** \brief Get the shell command type object
** 
** \param type 
** \return enum shell_type 
*/
enum shell_type get_shell_command_type(int type);

#endif /* PARSER_UTILS_H */
