#include "lexer/lexer.h"
#include "parser/parser.h"
#include "lexer/token.h"

struct parser *init_parser(struct lexer *lexer)
{
    struct parser *parser = malloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer)
    return parser;
}

struct token *get_next_token(struct parser *p)
{
    if (p == NULL)
        return NULL;
    if (p->current_toke->next->type == EOF)
        return NULL;
    return p->current_token->next;
}

void parser_eat(struct parser *p)
{
    while (p->current_token->value == TOK_NEWLINE)
        p->current_token = get_next_token;
}

static bool parse_look_ahead(struct parser *p, struct token *expected_token)
{
    if(lexer != NULL)
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
    struct token *token = peek(p->lexer);
    struct token *next = get_next_token(p);

    if (!p->lexer || !ast)
        return true;
    else
    {
        if (is_type(token, TOK_NEWLINE))
            return false;           
        else if (is_type(token, TOK_EOF))
            return false;
        else if
        {
            if (parse_list(lexer, ast) == false && is_type(next, TOK_EOF))
                return false;
            else if (parse_list(lexer, ast) == false && !is_type(next, TOK_EOF))
                return true;
            else if (parse_list(lexer, ast) == true && is_type(next, TOK_EOF))
                return true;
            else
                return true;
        }
        else
        {
            if (parse_list(lexer,ast) == false && is_type(next, TOK_NEWLINE))
                return false;
            else if (parse_list(lexer,ast) == false && !is_type(next, TOK_NEWLINE))
                return true;
            else if (parse_list(lexer,ast) == true && is_type(next, TOK_NEWLINE))
                return true;
            else
                return true;
        }
    }
}

static bool parse_list(struct parser *parser, struct ast_node *ast)
{
    struct token *current = parser->current_token;
    if (lexer == NULL || ast == NULL)
    {
        return true;
    }
    do
    {
        if (parse_and_or(parser, ast))
            return true;
        if (parser->current_token->type != TOK_SEMI && strcmp(parser->current_token->value, "&") != 0)
            return true;
        parser->current_token = parser->current_token->next;
    } while(!is_type(next, TOK_NEWLINE) && !is_type(next, TOK_EOF))
    return false;
}

static bool parse_and_or(struct parser *parser, struct ast_node *ast)
{
    if (parser == NULL || ast == NULL)
        return true;
    if (parse_pipeline(parser, ast))
        return true;
    while (parser->current_token->type == TOK_PIPE || parser->current_token->type == TOK_OR)
    {
        parser_eat(parser);
        if (parse_pipeline(parser, ast))
            return true;
    }
    return false;
}

static bool parse_pipeline(struct parser *parser, struct ast_node *ast)
{
    if(parser == NULL || ast == NULL)
        return true;
    if (parser->current_token->type == TOK_PIPE)
    {   
        //créer le node not
        get_next_token(parser);
    }
    if (parse_command(parser, ast))
    {
        while (parser->current_token->type == TOK_PIPE)
        {
            //créer le node pipe
            parser_eat(parser);
            if (!parse_command(parser, ast)
                return false;
        }
        return true;
    }
    else
        return false;
}

static bool parse_command(struct parser *parser, struct ast_node *ast)
{
    if(!parser || !ast) 
    {
        return true;
    }
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
    return false;
    
}

static bool parse_simple_command(struct parser *parser, struct ast_node *ast)
{
    struct token *current = NULL;
    if(parser == NULL || ast == NULL)
    {
        return true;
    }
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
            )
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