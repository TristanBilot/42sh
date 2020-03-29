#include "lexer/lexer.h"
#include "parser.h"

static bool parse_look_ahead(struct lexer *lexer, struct token *expected_token){
    if(lexer != NULL)
    {
        struct token *next_token = peek(struct lexer *lexer);
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

    if (parse_input(lexer, ast))
        printf("Parser error\n");

    return ast;
}
/* Pour les fonctions qui suivent, on a admis qu'on retourne False si on ne trouve pas d'erreur sinon on renvoie True */

static bool parse_input(struct lexer *lexer, struct ast_node *ast)
{
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
    else
    {
        struct token *next_token = peek(lexer);
        if (next_token->value == "\n")
        {
            return false;           
        }
        else if (next_token->value == "\0")
        {
            return false;
        }
        else if
        {
            struct token *t = new_token_type(2);    // = TOKEN EOF
            if (parse_list(lexer,ast) == false && parse_look_ahead(lexer, t) == true)
            {
                return false;
            }
            else if (parse_list(lexer,ast) == false && parse_look_ahead(lexer,t) == false)
            {
                return true;
            }
            else if (parse_list(lexer,ast) == true && parse_look_ahead(lexer,t) == true)
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

            struct token *t = new_token_type(1);    // = TOKEN NEWLINE
            if (parse_list(lexer,ast) == false && parse_look_ahead(lexer, t) == true)
            {
                return false;
            }
            else if (parse_list(lexer,ast) == false && parse_look_ahead(lexer,t) == false)
            {
                return true;
            }
            else if (parse_list(lexer,ast) == true && parse_look_ahead(lexer,t) == true)
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


static bool parse_list(struct lexer *lexer, struct ast_node *ast)
{
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
    
    do
    {
        if (parse_and_or(lexer, ast))
            return true;
        if (peek(lexer)->value != ";" && peek(lexer)->value != "&")
            return true;
        pop(lexer);
    } while(peek(lexer)->value != "\n" && peek(lexer)->value != "\0")
    return false;
}

static bool parse_and_or(struct lexer *lexer, struct ast_node *ast)
{
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
    if (parse_pipeline(lexer, ast))
        return true;
    while(peek(lexer)->value == "&&" || peek(lexer)->value == "||")
    {
        if (peek(lexer)->value != "" && peek(lexer)->value != "&")
            return true;
        pop(lexer);
    }
    return false;
}

static bool parse_pipeline(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
    if (parse_command(lexer, ast))
}

static bool parse_command(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_simple_command(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_shell_command(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}
static bool parse_funcdec(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_redirection(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_prefix(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_element(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_coumpound_list(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_for(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_while(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_until(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_case(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_rule_if(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_else_clause(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_do_group(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_case_clause(struct lexer *lexer, struct ast_node *ast){
    if(lexer == NULL || ast == NULL)
    {
        return true;
    }
}

static bool parse_case_item(struct lexer *lexer, struct ast_node *ast)
{
    if(lexer == NULL || ast == NULL)
{
        return true;
}

}