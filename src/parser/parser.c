#include "lexer/lexer.h"
#include "parser/parser.h"
#include "lexer/token.h"
#include <stdlib.h>
#include <stdio.h>


struct parser *init_parser(struct lexer *lexer)
{
    struct parser *parser = malloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer);
    return parser;
}

struct token *get_next_token(struct parser *p)
{
    if (p == NULL)
        return NULL;
    if (is_type(p->current_token->next, EOF))
        return NULL;
    return p->current_token->next;
}

void parser_eat(struct parser *p)
{
    while (p->current_token->value == TOK_NEWLINE)
        p->current_token = get_next_token(p);
}

void next_token(struct parser *parser)
{
    if (!parser->current_token || !parser->current_token->next)
        return;
    parser->current_token = parser->current_token->next;
}

static bool parse_look_ahead(struct parser *p, struct token *expected_token)
{
    if (lexer != NULL)
    {
        struct token *next_token = get_next_token(p);
        if(expected_token == next_token)
            return true;
        else
            return false;
    }
    else
        return false;
}

void *parse(struct lexer *lexer)
{
    //struct node_input *ast = init_node_input();                           // = fonction ast_node_input a redefinir
    if (!lexer) //|| !ast)
        error("Struct error");
    struct parser *parser = init_parser(lexer);
    if (parse_input(parser, ast))
        error("Parser error");
    return ast;
}
/* Pour les fonctions qui suivent, on a admis qu'on retourne False si on ne trouve pas d'erreur sinon on renvoie True */

static bool parse_input(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    if (!parser->lexer) //|| !ast)
        return true;
    if (is_type(parser->current_token, TOK_NEWLINE) ||
        is_type(parser->current_token, TOK_EOF))
        return false;
    if (!parse_list(parser, ast))
        if (is_type(parser->current_token, TOK_EOF) ||
            is_type(parser->current_token, TOK_NEWLINE))
            return false;
    return true;
}

static bool parse_list(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    if (!lexer || !ast)
        return true;
    do
    {
        if (parse_and_or(parser, ast))
            return true;
        if (!is_type(parser->current_token->type, TOK_SEMI) && 
            !is_type(parser->current_token->type, TOK_SEPAND))
            return true;
        next_token(parser);
    } while(!is_type(parser->current_token->type, TOK_NEWLINE) && 
        !is_type(parser->current_token->type, TOK_EOF));
    return false;
}

static bool parse_and_or(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    if (!parser) //|| !ast)
        return true;
    if (parse_pipeline(parser, ast))
        return true;
    while (is_type(parser->current_token->type, TOK_AND) ||
        is_type(parser->current_token->type, TOK_OR))
    {
        parser_eat(parser);
        if (parse_pipeline(parser, ast))
            return true;
    }
    return false;
}

static bool parse_pipeline(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    if (!parser) //|| !ast)
        return true;
    //créer node pipeline
    if (is_type(parser->current_token->type, TOK_NOT))
    {   
        // node_pipeline->not = true
        next_token(parser);
    }
    if (!parse_command(parser, ast))
    {
        while (is_type(parser->current_token->type, TOK_PIPE))
        {
            //créer le node pipe
            parser_eat(parser);
            if (parse_command(parser, ast)
                return true;
            next_token(parser);
            //créer le node command
        }
        return false;
    }
    return true;
}

static bool parse_command(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    if(!parser) //|| !ast) 
        return true;
    if (parse_shell_command(parser, ast))
    {
        if (parse_funcdec(parser, ast))
        {
            if (parse_simple_command(parser, ast))
                return true;
        
        }
        else
        {
            do
            {
                parse_redirection(parser, ast);

            } while(is_type(p->current->next,TOK_DLESSDASH) ||
                is_type(p->current->next,TOK_DLESS) ||
                is_type(p->current->next,TOK_LESSGREAT) ||
                is_type(p->current->next,TOK_LESSAND) ||
                is_type(p->current->next,TOK_LESS) ||
                is_type(p->current->next,TOK_DGREAT) ||
                is_type(p->current->next,TOK_GREATAND) ||
                is_type(p->current->next,TOK_CLOBBER) ||
                is_type(p->current->next,TOK_GREAT) ||
            )
        }
    }
    // redirections
    return false;
    
}

static bool parse_simple_command(struct parser *parser, struct ast_node *ast)
{
    (void)ast;
    struct token *current = NULL;
    if (!parser || !ast)
        return true;
    if (parse_prefix(parser, ast))
    {
        do
        {
            if (parse_element(parser, ast))
                return true;
            current = parser->current_token;
        } while (!is_type(current, TOK_PIPE) && 
            !is_type(current, TOK_AND) &&
            !is_type(current, TOK_OR) &&
            !is_type(current, TOK_SEMI) &&
            !is_type(current, TOK_SEPAND) 
            );
    }
    else
    {
        while (!is_type(current, TOK_PIPE) && 
            !is_type(current, TOK_AND) &&
            !is_type(current, TOK_OR) &&
            !is_type(current, TOK_SEMI) &&
            !is_type(current, TOK_SEPAND) 
            // Not element
            )
        {
            //parse prefix
        }
        //parse element
    }

}

static bool parse_shell_command(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_funcdec(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_redirection(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_prefix(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_element(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_coumpound_list(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_rule_for(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_rule_while(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_rule_until(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_rule_case(struct parser *parser, struct ast_node *ast){
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_rule_if(struct parser *parser, struct ast_node *ast){
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_else_clause(struct parser *parser, struct ast_node *ast){
    if(!parser || !ast)
    {
        return true;
    }
}

static bool parse_do_group(struct parser *parser, struct ast_node *ast){
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_case_clause(struct parser *parser, struct ast_node *ast){
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}

static bool parse_case_item(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast)
    {
        return true;
    }
    return false;
}