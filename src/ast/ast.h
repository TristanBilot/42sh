/**
** \file ast.h
** \author Team
** \brief Define ast and parser structures
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/

#ifndef AST_H
#define AST_H

#include <stdbool.h>
//#include "parser/parser.h"

// struct ast_node
// {
//     enum node_type
//     {
//         NODE_IF,                                // = KEYWORD
//         NODE_SIMPLECOMMAND,                     // = TOKEN COMMAND
//         NODE_WORD,                              // = WORD
//         NODE_LOGICAL
//     } type;

//     union
//     {
//         struct ast_node_if *ast_node_if;
//         struct ast_node_command *ast_node_simplecommand;
//         struct ast_node_word *ast_node_word;
//         struct ast_node_logical *ast_node_logical;
//     } data;
// };

struct parser
{
    struct lexer *lexer;
    struct token *current_token;
};

struct node_input
{
    struct node_list *node_list;
};

struct node_list
{
    struct node_and_or *and_or;
    struct node_list *next_sibling;
    enum type
    {
        SEMI,
        SEPAND,
        NONE
    } type;
};

struct node_and_or
{
    bool is_final;
    union left
    {
        struct node_pipeline *pipeline;
        struct node_and_or *and_or;
    } left;
    struct node_pipeline *right;
    enum type_logical
    {
        AND,
        OR
    } type;
};

struct node_pipeline
{
    bool is_not;
    struct node_command *command;
    struct node_pipeline *next_sibling;
};

struct node_command
{
    enum command_token
    {
        SIMPLE_COMMAND,
        SHELL_COMMAND,
        FUNCDEC
    } type;

    union command
    {
        struct node_simple_command *simple_command;
        struct node_shell_command *shell_command;
        struct node_funcdec *funcdec;
    } command;
    struct node_redirection *redirections;
};

struct node_simple_command
{
    bool to_export;
    struct node_prefix *prefixes; /* LIST */
    struct node_element *elements; /* LIST */
};

struct node_shell_command
{
    enum type_clause
    {
        C_BRACKETS,
        PARENTHESIS,
        RULE
    } type;

    union shell
    {
        struct node_compound_list *compound_list;
        struct node_for *rule_for;
        struct node_while *rule_while;
        struct node_until *rule_until;
        struct node_case *rule_case;
        struct node_if *rule_if;
    } shell;
    enum shell_type /* NEED IMPLEMENTATION !!!!!!!!! */
    {
        FOR,
        WHILE,
        UNTIL,
        CASE,
        IF
    } shell_type;
};

struct node_funcdec
{
    bool is_function;
    char *function_name;
    struct node_shell_command *shell_command;
};

struct node_redirection
{
    // enum type_redirection{
    //     DLESSDASH,	// <<-
    //     DLESS,		// <<
    //     LESSGREAT,	// <>
    //     LESSAND,	// <&
    //     LESS,		// <
    //     DGREAT,		// >>
    //     GREATAND,	// >&
    //     CLOBBER,	// >|
    //     GREAT		// >
    // }type;
    unsigned int type;
    char *left;
    char *right;
    struct node_redirection *next;
};

struct node_prefix
{
    struct node_prefix *next;
    enum type_prefix
    {
        REDIRECTION,
        ASSIGMENT_WORD
    } type;
    union prefix
    {
        struct assigment_word
        {
            char *variable_name;
            char *value;
        } *assigment_word; /* MAYBE CREATE A POINTER */
        struct node_redirection *redirection;
    } prefix;
};

struct node_element
{
    struct node_element *next;
    enum type_element
    {
        TOKEN_REDIRECTION,
        WORD
    } type;
    union element
    {
        char *word;
        struct node_redirection *redirection;
    } element;
};

struct node_compound_list
{
    struct node_and_or *and_or;
    struct node_compound_list *next_sibling;
};

struct node_while
{
    struct node_compound_list *condition;
    struct node_do_group *body;
};

struct node_until
{
    struct node_compound_list *condition;
    struct node_do_group *body;
};

struct node_case
{
    bool is_case_clause;
    char *word;
    struct node_case_clause *case_clause;
};

struct node_if                              // INSTANCE KEYWORD
{
    struct node_compound_list *condition;   // the condition
    struct node_compound_list *if_body;   // on peut mettre directement ici la liste de struct elif  // the body of the if
    struct node_else_clause *else_clause;   // the body of the else, may be NULL
};

/* ONLY FOR NODE_FOR */
struct range
{
    char *value;
    struct range *next;
};
struct node_for
{
    char *variable_name;
    struct range *range;
    struct node_do_group *body;
};

struct node_else_clause
{
    enum else_clause
    {
        ELIF,
        ELSE
    } type;
    union
    {
        struct node_if *elif; 
        struct node_compound_list *else_body;
    } clause;
};

struct node_do_group
{
    struct node_compound_list *body;
};

struct node_case_clause
{
    struct node_case_item *case_item;
    struct node_case_clause *next;
};

/* ONLY FOR NODE_CASE_ITEM */
struct word_list
{
    char *word;
    struct word_list *next;
};

struct node_case_item
{
    struct word_list *words;
    /*enum item
    {
        COMPOUND,
        NEXT
    } type;*/
    struct node_compound_list *compound_list;
    /*union next
    {
        
        struct node_case_item *next;
    } next;*/
};


/**
** \brief build node input
**
** \return struct node_input* 
*/
struct node_input *build_input(void);
/**
** \brief build node list
**
** \return struct node_list* 
*/
struct node_list *build_list(void);
/**
** \brief build node and_or_final
**
** \param is_and 
** \param left 
** \param right 
** \return struct node_and_or* 
*/
struct node_and_or *build_and_or_final(bool is_and, struct node_pipeline *left, struct node_pipeline *right);
/**
** \brief build node_and_or_merge
**
** \param is_and 
** \param left 
** \param right 
** \return struct node_and_or* 
*/
struct node_and_or *build_and_or_merge(bool is_and, struct node_and_or *left, struct node_pipeline *right);
/**
** \brief build node pipeline
**
** \param is_not 
** \return struct node_pipeline* 
*/
struct node_pipeline *build_pipeline(bool is_not);
/**
** \brief build command
**
** \return struct node_command* 
*/
struct node_command *build_command(void);
/**
** \brief build simple command
**
** \return struct node_simple_command* 
*/
struct node_simple_command *build_simple_command(void);
/**
** \brief build shell command
**
** \param parser 
** \return struct node_shell_command* 
*/
struct node_shell_command *build_shell_command(struct parser *parser);
/**
** \brief build node funcdec
**
** \return struct node_funcdec* 
*/
struct node_funcdec *build_funcdec();
/**
** \brief build node redirection
**
** \param parser 
** \return struct node_redirection* 
*/
struct node_redirection *build_redirection(struct parser *parser);
/**
** \brief build node prefix
**
** \param parser 
** \return struct node_prefix* 
*/
struct node_prefix *build_prefix(struct parser *parser);
/**
** \brief build node element
**
** \param parser 
** \return struct node_element* 
*/
struct node_element *build_element(struct parser *parser);
/**
** \brief build node compound list
**
** \return struct node_compound_list* 
*/
struct node_compound_list *build_compound_list(void);
/**
** \brief build node while
**
** \return struct node_while* 
*/
struct node_while *build_while(void);
/**
** \brief build node until
**
** \return struct node_until* 
*/
struct node_until *build_until(void);
/**
** \brief build node case
**
** \param parser 
** \return struct node_case* 
*/
struct node_case *build_case(struct parser *parser);
/**
** \brief build node if
**
** \return struct node_if* 
*/
struct node_if *build_if(void);
/**
** \brief build node for
**
** \return struct node_for* 
*/
struct node_for *build_for(void);
/**
** \brief build node else clause
**
** \param parser 
** \return struct node_else_clause* 
*/
struct node_else_clause *build_else_clause(struct parser *parser);
/**
** \brief build do group
**
** \return struct node_do_group* 
*/
struct node_do_group *build_do_group(void);
/**
** \brief build node case clause
**
** \return struct node_case_clause* 
*/
struct node_case_clause *build_case_clause(void);
/**
** \brief build node case item
**
** \return struct node_case_item* 
*/
struct node_case_item *build_case_item(void);
/**
** \brief build node export
**
** \param flag 
** \return struct node_export* 
*/
//struct node_export *build_export(unsigned int flag);

#endif /* ! AST_H */
