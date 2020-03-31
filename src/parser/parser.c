#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/string.h"
#include "../utils/parser_utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_FLAG true
#define DEBUG(msg) if (DEBUG_FLAG) \
                        printf("%s\n", msg);

struct parser *init_parser(struct lexer *lexer)
{
    DEBUG("Erreur sur la fonction : init_parser\n");
    struct parser *parser = malloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer);
    return parser;
}

struct token *get_next_token(struct parser *p)
{
    DEBUG("Erreur sur la fonction : get_next_token\n");
    if (p == NULL)
        return NULL;
    if (is_type(p->current_token->next, EOF))
        return NULL;
    return p->current_token->next;
}

void parser_eat(struct parser *p)
{
    DEBUG("Erreur sur la fonction : parser_eat\n");
    while (is_type(p->current_token, TOK_NEWLINE))
        p->current_token = get_next_token(p);
}

void next_token(struct parser *parser)
{
    DEBUG("Erreur sur la fonction : next_token\n");
    if (!parser->current_token || !parser->current_token->next)
        return;
    parser->current_token = parser->current_token->next;
}

/*static bool parse_look_ahead(struct parser *p, struct token *expected_token)
{
    if (p != NULL)
    {
        struct token *next_token = get_next_token(p);
        if(expected_token == next_token)
            return true;
        else
            return false;
    }
    else
        return false;
}*/

void *parse(struct lexer *lexer)
{
    DEBUG("Erreur sur la fonction : parse\n");
    struct ast_node *ast = NULL;//init_node_input();                           // = fonction ast_node_input a redefinir
    if (!lexer) //|| !ast)
        error("Struct error");
    struct parser *parser = init_parser(lexer);
    if (parse_input(parser, ast))
        error("Parser error");
    return ast;
}
/* Pour les fonctions qui suivent, on a admis qu'on retourne False si on ne trouve pas d'erreur sinon on renvoie True */

bool parse_input(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_input\n");
    if (!parser->lexer) //|| !ast)
        return true;
    if (is_type(parser->current_token, TOK_NEWLINE) ||
        is_type(parser->current_token, TOK_EOF)){
            //printf("SUCCESS BUT NO TREE \n");
            return false;
        }
    if (!parse_list(parser, ast))
        if (is_type(parser->current_token, TOK_EOF) ||
            is_type(parser->current_token, TOK_NEWLINE))
            {
                //printf("SUCCESS BUT NO TREE \n");
                return false;
            }
    return true;
}

bool parse_list(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_list\n");
    (void)ast;
    if (!parser) //|| !ast)
        return true;
    do
    {
        if (parse_and_or(parser, ast))
            return true;
        if (!is_type(parser->current_token, TOK_SEMI) && 
            !is_type(parser->current_token, TOK_SEPAND) &&
            !is_type(parser->current_token, TOK_NEWLINE) && 
            !is_type(parser->current_token, TOK_EOF))
            return true;
        if (is_type(parser->current_token, TOK_SEMI) ||
            is_type(parser->current_token, TOK_SEPAND))
            next_token(parser);
    } while(!is_type(parser->current_token, TOK_NEWLINE) && 
        !is_type(parser->current_token, TOK_EOF));
    return false;
}

bool parse_and_or(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_and_or\n");
    (void)ast;
    if (!parser) //|| !ast)
        return true;
    if (parse_pipeline(parser, ast))
        return true;
    while (is_type(parser->current_token, TOK_AND) ||
        is_type(parser->current_token, TOK_OR))
    {
        parser_eat(parser);
        if (parse_pipeline(parser, ast))
            return true;
    }
    return false;
}

bool parse_pipeline(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_pipeline\n");
    (void)ast;
    if (!parser) //|| !ast)
        return true;
    //créer node pipeline
    if (is_type(parser->current_token, TOK_NOT))
    {   
        // node_pipeline->not = true
        next_token(parser);
    }
    if (!parse_command(parser, ast))
    {
        while (is_type(parser->current_token, TOK_PIPE))
        {
            //créer le node pipe
            parser_eat(parser);
            if (parse_command(parser, ast))
                return true;
            next_token(parser);
            //créer le node command
        }
        return false;
    }
    return true;
}

bool parse_command(struct parser *p, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_command\n");
    struct token *current = p->current_token;
    if (parse_shell_command(p, ast))
    {
        p->current_token = current;
        if (parse_funcdec(p, ast))
        {
            p->current_token = current;
            return parse_simple_command(p, ast);
        }
        while (is_redirection(p->current_token))
            if (parse_redirection(p, ast))
                return true;
    }
    while (is_redirection(p->current_token))
        if (parse_redirection(p, ast))
            return true;
    return false;
}

bool parse_simple_command(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_simple_command\n");
    struct token *current = parser->current_token;
    if (parse_prefix(parser, ast))
    {
        parser->current_token = current;
        if (parse_element(parser, ast))
            return true;
        current = parser->current_token;
        while (!parse_element(parser, ast))
            current = parser->current_token;
        parser->current_token = current;
    }
    else
    {
        while (!parse_prefix(parser, ast))
            current = parser->current_token;
        parser->current_token = current;
        while (!parse_element(parser, ast))
            current = parser->current_token;
        parser->current_token = current;
    }
    return false;
}

bool parse_shell_command(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_shell_command\n");
    struct token *current = parser->current_token;
    if (is_type(current, TOK_LCURL) || is_type(current, TOK_LPAREN))
    {
        next_token(parser);
        if (parse_compound_list(parser, ast))
            return true;
        current = parser->current_token;
        return (is_type(current, TOK_RCURL) || is_type(current, TOK_RPAREN)) ? false : true;
    }
    if (parse_rule_for(parser, ast))
        return true;
    if (parse_rule_while(parser, ast))
        return true;
    if (parse_rule_until(parser, ast))
        return true;
    if (parse_rule_case(parser, ast))
        return true;
    if (parse_rule_if(parser, ast))
        return true;
    return false;
}

bool parse_funcdec(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_funcdec\n");
    struct token *current = parser->current_token;
    if (strcmp("function", current->value) == 0)
        next_token(parser);
    //créer node function
    next_token(parser);
    if (!is_type(current, TOK_WORD))
        return true;
    //créer node word
    next_token(parser);
    if (!is_type(current, TOK_LPAREN))
        return true;
    next_token(parser);
    if (!is_type(current, TOK_RPAREN))
        return true;
    parser_eat(parser);
    if (parse_shell_command(parser, ast))
        return true;
    //créer shell_command
    //créer node funcdec
    return false;
}

bool parse_redirection(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_redirection\n");
    if (!(is_type(parser->current_token, TOK_IONUMBER))){
        printf("%s\n",parser->current_token->value);
        return true;
    }
        
    next_token(parser);
    struct token *curr = parser->current_token;
    if (!is_redirection(curr)){
        printf("%s\n","test2");
        return true;
    }
        
    next_token(parser);
    curr = parser->current_token;
    next_token(parser);
    printf("%d\n", curr->type);
    if(!(is_type(curr, TOK_WORD)))
        printf("%s\n","test3");
    return (!(is_type(curr, TOK_WORD) /*|| is_type(curr, TOK_HEREDOC) */ )) ? true : false;
}

bool parse_prefix(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_prefix\n");
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        next_token(parser);
        return false;
    }
    if (is_type(parser->current_token, TOK_IONUMBER))
    {
        return parse_redirection(parser, ast) ? true : false;
    }
    return true;
}

bool parse_element(struct parser *parser, void *ast)
{
    DEBUG("Erreur sur la fonction : parse_element\n");
    if (is_type(parser->current_token, TOK_WORD))
    {
        next_token(parser);
        return false;
    }
    if (is_type(parser->current_token, TOK_IONUMBER))
    {
        return parse_redirection(parser, ast) ? true : false;
    }
    return true;
}

bool parse_compound_list(struct parser *parser, void *ast)
{
    parser_eat(parser);
    if (parse_and_or(parser, ast))
        return true;
    while (is_type(parser->current_token, TOK_SEMI) ||
        is_type(parser->current_token, TOK_SEPAND) ||
        is_type(parser->current_token, TOK_NEWLINE)
        )
    {
        parser_eat(parser);
        if (parse_and_or(parser, ast))
        {
            parser_eat(parser);
            return false;
        }
    }
    next_token(parser);
    return false;
}

bool parse_rule_for(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (!(is_type(current, KW_FOR)))
        return true;
    next_token(parser);
    current = parser->current_token;
    if (!(is_type(current, TOK_WORD)))
        return true;
    next_token(parser);
    // parser_eat(parser);
    if (is_type(current, TOK_SEMI))
    {
        next_token(parser);
        current = parser->current_token;
    }
    else
        parser_eat(parser);
    if (!(is_type(current, KW_IN)))
        return true;
    next_token(parser);
    current = parser->current_token;

    while (is_type(current, TOK_WORD))
    {
        next_token(parser);
        current = parser->current_token;
    }
    if (!(is_type(current, TOK_SEMI) || is_type(current, TOK_NEWLINE)))
        return true;
    next_token(parser);
    current = parser->current_token;
    parser_eat(parser);
    return parse_do_group(parser, ast) ? true : false;
}

    
bool parse_rule_while(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (is_type(current, KW_WHILE))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, ast))
            return true;
        return (parse_do_group(parser, ast)) ? true : false;
    }
    return true;
}

bool parse_rule_until(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (is_type(current, KW_UNTIL))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, ast))
            return true;
        return (parse_do_group(parser, ast)) ? true : false;
    }
    return true;
}

bool parse_rule_case(struct parser *parser, void *ast)
{
    if (!is_type(parser->current_token, KW_CASE))
        return true;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_IN))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_ESAC))
    {
        if (parse_case_clause(parser, ast))
            return true;
        if (!is_type(parser->current_token, KW_ESAC))
            return true;
    }
    next_token(parser);
    return false;
}

bool parse_rule_if(struct parser *parser, void *ast)
{
    struct token *curr = parser->current_token;
    if (!is_type(curr, KW_IF))
        return true;
    next_token(parser);
    
    if (parse_compound_list(parser, ast))
        return true;
    curr = parser->current_token;
    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);

    if (parse_compound_list(parser, ast))
        return true;
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE))
        return parse_else_clause(parser, ast) ? true : false;
    
    next_token(parser);
    return (!(is_type(curr, KW_FI))) ? true : false;
}

bool parse_else_clause(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (is_type(current, KW_ELSE))
        return (parse_compound_list(parser, ast)) ? true : false;
    if (is_type(current, KW_ELIF))
    {
        if(parse_compound_list(parser, ast))
            return true;
        current = parser->current_token;
        if(is_type(current, KW_THEN)){
            if (parse_compound_list(parser, ast))
                return true;
            current = parser->current_token;
            if (is_type(current, KW_ELSE))
                parse_else_clause(parser, ast);
            else
                return false;
        }
        else
            return true;
    }
    return true;
}

bool parse_do_group(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (is_type(current, KW_DO))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, ast))
            return true;
        current = parser->current_token;
        return (is_type(current, KW_DONE) ? true : false);
    }
    return true;
}

bool parse_case_clause(struct parser *parser, void *ast)
{
    struct token *current = parser->current_token;
    if (parse_case_item(parser, ast))
        return true;
    next_token(parser);
    while (is_type(current, KW_DSEMI))
    {
        //créer node double semicol
        parser_eat(parser);
        if (parse_case_item(parser, ast))
            return true;
        next_token(parser);
    }
    if (is_type(current, KW_DSEMI))
    {
        // break;
        //créer node semicolon
    }
    parser_eat(parser);
    return false;
}

bool parse_case_item(struct parser *parser, void *ast)
{
    if (is_type(parser->current_token, TOK_LPAREN))
        next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    next_token(parser);
    while (is_type(parser->current_token, TOK_PIPE))
    {
        next_token(parser);
        if (!is_type(parser->current_token, TOK_WORD))
            return true;
        next_token(parser);
    }
    if (!is_type(parser->current_token, TOK_RPAREN))
        return true;
    parser_eat(parser);
    struct token *current = get_next_token(parser);
    if (!is_type(current, KW_DSEMI) &&
        !is_type(current, TOK_NEWLINE) &&
        !is_type(current, KW_ESAC))
    {
        if (parse_compound_list(parser, ast))
            return true;
    }
    next_token(parser);
    return false;
}
