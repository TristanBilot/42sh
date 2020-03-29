#include "lexer/lexer.h"
#include "parser.h"

struct parser *init_parser(struct lexer *lexer)
{
    struct parser *parser = malloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer)
    return parser;
}


struct token *get_next_token(struct parser *p){
    if(p == NULL)
        return NULL;
    else
    {
        return p->current_token->next;
    }
}

static bool parse_look_ahead(struct parser *p, struct token *expected_token){
    if(lexer != NULL)
    {
        struct token *next_token = get_next_token(p);
        if(expected_token == next_token)
        {
            return true;
        }
        else
        {
            return false;
        }  
    }
    else
    {
        return false;
    }
}

struct ast_node *parser(struct lexer *lexer)
{
    struct ast_node *ast = ast_node_init();
    if (!lexer || !ast)
        printf("Struct error\n");
    struct parser *parser = init_parser(lexer);
    if (parse_input(parser, ast))
        printf("Parser error\n");
    return ast;
}
/* Pour les fonctions qui suivent, on a admis qu'on retourne False si on ne trouve pas d'erreur sinon on renvoie True */

static bool parse_input(struct parser *p, struct ast_node *ast)
{
    if(p->lexer == NULL || ast == NULL)
    {
        return true;
    }
    else
    {
        struct token *token = peek(p->lexer);
        if (token->value == "\n")
        {
            return false;           
        }
        else if (token->value == "\0")
        {
            return false;
        }
        else if
        {
            
            if (parse_list(lexer, ast) == false && strcmp(get_next_token(p)->value,"\0") == 0)
            {
                return false;
            }
            else if (parse_list(lexer, ast) == false && strcmp(get_next_token(p)->value,"\0") == 1)
            {
                return true;
            }
            else if (parse_list(lexer, ast) == true && strcmp(get_next_token(p)->value,"\0") == 0)
            {
                return true;
            }
            else
            {
                return true;
            }
            
            
        }
        else
        {
            if (parse_list(lexer,ast) == false && strcmp(get_next_token(p)->value,"\n") == 0)
            {
                return false;
            }
            else if (parse_list(lexer,ast) == false && strcmp(get_next_token(p)->value,"\n") == 1)
            {
                return true;
            }
            else if (parse_list(lexer,ast) == true && strcmp(get_next_token(p)->value,"\n") == 0)
            {
                return true;
            }
            else
            {
                return true;
            }
        }
    }
}


static bool parse_list(struct parser *parser, struct ast_node *ast)
{
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
    do
    {
        if (parse_and_or(parser, ast))
            return true;
        if (strcmp(parser->current_token->value, ";") != 0 && strcmp(parser->current_token->value, "&") != 0)
            return true;
        parser->current_token = parser->current_token->next;
    } while(strcmp(parser->current_token->value, "\n") != 0 && strcmp(parser->current_token->value, "\0") != 0)
    return false;
}

static bool parse_and_or(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
        return true;
    if (parse_pipeline(parser, ast))
        return true;
    while(strcmp(parser->current_token->value, "&&") == 0 || strcmp(parser->current_token->value, "||") == 0)
    {
        while (parser->current_token && strcmp(parser->current_token->value, "\n") == 0)
            parser->current_token = parser->current_token->next;
        if (parse_pipeline(parser, ast))
            return true;
    }
    return false;
}

static bool parse_pipeline(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
        return true;
    if (strcmp(parser->current_token->value, "!") == 0)
    {   
        //créer le node not
        pop(parser->lexer);
    }
    if (parse_command(parser, ast))
    {
        if (strcmp("", "|")
        return true;
    }
    
}

static bool parse_command(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_simple_command(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_shell_command(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_funcdec(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_redirection(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_prefix(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_element(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_coumpound_list(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_for(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_while(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_until(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_case(struct parser *parser, struct ast_node *ast){
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_if(struct parser *parser, struct ast_node *ast){
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_else_clause(struct parser *parser, struct ast_node *ast){
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_do_group(struct parser *parser, struct ast_node *ast){
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_case_clause(struct parser *parser, struct ast_node *ast){
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_case_item(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
    {
        return true;
    }

}