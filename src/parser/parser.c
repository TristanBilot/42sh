#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../parser/parser.h"
#include "../ast/free.h"
#include "../ast/ast.h"
#include "../utils/parser_utils.h"
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"

#define DEBUG_FLAG false
#define DEBUG(msg) if (DEBUG_FLAG) \
                        printf("%s", msg);

struct parser *init_parser(struct lexer *lexer)
{
    DEBUG("init_parser\n");
    struct parser *parser = xmalloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer);
    return parser;
}

void free_parser(struct parser *p)
{
    if (p)
    {
        free_lexer(p->lexer);
        free(p);
    }
}

struct token *get_next_token(struct parser *p)
{
    DEBUG("get_next_token\n");
    if (p == NULL)
        return NULL;
    if (is_type(p->current_token->next, EOF))
        return NULL;
    return p->current_token->next;
}

void parser_comment(struct parser *p)
{
    if (is_type(p->current_token, TOK_COMM))
    while (!is_type(p->current_token, TOK_NEWLINE) && !is_type(p->current_token, TOK_EOF))
    {
        p->current_token = get_next_token(p);
    }
}

void parser_eat(struct parser *p)
{
    DEBUG("parser_eat\n");
    parser_comment(p);
    while (is_type(p->current_token, TOK_NEWLINE))
    {
        p->current_token = get_next_token(p);
        parser_comment(p);
    }
}

void next_token(struct parser *parser)
{
    if (!parser->current_token || !parser->current_token->next)
        return;
    parser->current_token = parser->current_token->next;
}


void *parse(struct lexer *lexer)
{
    DEBUG("parse\n");
    struct node_input *ast = NULL;//init_node_input();                           // = fonction ast_node_input a redefinir
    if (!lexer)
        error("Struct error");
    struct parser *parser = init_parser(lexer);

    if (parse_input(parser, &ast))
    {
        error("Parser error");

        //free_input(ast);
        //ast = NULL;
    }
    //free_parser(parser);
    return ast;
}
/* Pour les fonctions qui suivent, on a admis qu'on retourne False si on ne trouve pas d'erreur sinon on renvoie True */

bool parse_input(struct parser *parser, struct node_input **ast)
{
    DEBUG("parse_input\n");
    if (!parser->lexer)
    {
        return true;
    }
    parser_comment(parser);
    struct node_input *node = build_input(); //
    if ((is_type(parser->current_token, TOK_NEWLINE) &&
        is_type(parser->current_token->next, TOK_EOF)) ||
        is_type(parser->current_token, TOK_EOF))
    {
        *ast = node;
        return false;
    }
    //*ast = build_input();
    if (!parse_list(parser, &(node->node_list))) // &((*ast)->node_list)
    {
        parser_comment(parser);
        if (is_type(parser->current_token, TOK_EOF) ||
            (is_type(parser->current_token, TOK_NEWLINE) &&
            is_type(parser->current_token->next, TOK_EOF)))
        {
            *ast = node;
            return false;
        }
    }
    //free_list((*ast)->node_list);
    return true;
}

bool parse_list(struct parser *parser, struct node_list **ast)
{
    DEBUG("parse_list\n");
    if (!parser)
        return true;
    //*ast = build_list();
    struct node_list *node = build_list(); //
    // struct node_list *tmp = *ast;
    struct node_list *tmp = node; //
    bool is_first = true;
    do
    {
        if (is_first)
        {
            is_first = !is_first;
        }
        else
        {
            tmp->next_sibling = build_list();
            tmp = tmp->next_sibling;
        }
        if (parse_and_or(parser, &(tmp->and_or)))
        {
            // free_and_or(tmp->and_or);
            return true;
        }
        parser_comment(parser);
        if (!is_type(parser->current_token, TOK_SEMI) && 
            !is_type(parser->current_token, TOK_SEPAND) &&
            !is_type(parser->current_token, TOK_NEWLINE) && 
            !is_type(parser->current_token, TOK_EOF))
        {
            return true;
        }
        if (is_type(parser->current_token, TOK_SEMI))
        {
            tmp->type = SEMI;
            next_token(parser);
        }
        else if (is_type(parser->current_token, TOK_SEPAND))
        {
            tmp->type = SEPAND;
            next_token(parser);
        }
        else
            tmp->type = NONE;
        parser_comment(parser);
    } while (!is_type(parser->current_token, TOK_NEWLINE) && 
        !is_type(parser->current_token, TOK_EOF));
    *ast = node; //
    return false;
}

bool parse_and_or(struct parser *parser, struct node_and_or **ast)
{
    DEBUG("parse_and_or\n");
    if (!parser)
        return true;
    struct node_and_or *node_and_or = build_and_or_final(false, NULL, NULL);    // and_or: pipeline (('&&'|'||') ('\n')* pipeline)*
    
    if (parse_pipeline(parser, &(node_and_or->left.pipeline)))
    {
        // free_pipeline(node_and_or->left.pipeline);
        return true;
    }
    int count = 0;
    while (is_type(parser->current_token, TOK_AND) ||
        is_type(parser->current_token, TOK_OR))
    {
        bool is_and = is_type(parser->current_token, TOK_AND);
        next_token(parser);
        parser_eat(parser);
        if (parse_pipeline(parser, &(node_and_or->right)))
        {
            //free_pipeline(node_and_or->right);
            return true;
        }
        if (count == 0)
        {
            // free left.pipeline
            node_and_or->left.and_or = build_and_or_final(is_and, node_and_or->left.pipeline, node_and_or->right);
        }
        else
        {
            node_and_or->left.and_or = build_and_or_merge(is_and, node_and_or->left.and_or, node_and_or->right);
        }
        count++;
    }
    if (count <= 0)
        *ast = node_and_or;
    else
        *ast = node_and_or->left.and_or;
    return false;
}

bool parse_pipeline(struct parser *parser, struct node_pipeline **ast)
{
    DEBUG("parse_pipeline\n");
    if (!parser)
        return true;
    bool is_not = false;
    if (is_type(parser->current_token, TOK_NOT))
    {
        is_not = true;
        next_token(parser);
    }
    // *ast = build_pipeline(is_not);
    struct node_pipeline *new = build_pipeline(is_not); //
    //struct node_pipeline *tmp = *ast;
    struct node_pipeline *tmp = new; //
    if (!parse_command(parser, &(new->command))) // &((*ast)->command)
    {
        while (is_type(parser->current_token, TOK_PIPE))
        {
            tmp->next_sibling = build_pipeline(false);
            tmp = tmp->next_sibling;
            next_token(parser);
            parser_eat(parser);
            if (parse_command(parser, &(tmp->command)))
            {
                //free_command(tmp->command);
                return true;
            }
        }
        *ast = new; //
        return false;
    }
    // else
    //     free_command((*ast)->command);
    return true;
}

bool parse_command(struct parser *p, struct node_command **ast)
{
    DEBUG("parse_command\n");
    struct token *current = p->current_token;
    //*ast = build_command();
    struct node_command *new = build_command(); //
    if (parse_shell_command(p, &(new->command.shell_command))) //&((*ast)->command.shell_command)
    {
        //free_shell_command((*ast)->command.shell_command);
        p->current_token = current;
        if (parse_funcdec(p, &(new->command.funcdec))) //&((*ast)->command.funcdec))
        {
            //free_funcdec((*ast)->command.funcdec);
            // FREE ast->command.funcdec
            p->current_token = current;
            if (parse_simple_command(p, &(new->command.simple_command))) //&((*ast)->command.simple_command)
            {
                //free_simple_command((*ast)->command.simple_command);
                return true;
            }
            //(*ast)->type = SIMPLE_COMMAND;
            new->type = SIMPLE_COMMAND; //
            *ast = new; //
            return false;
        }
        //(*ast)->type = FUNCDEC;
        new->type = FUNCDEC; //
        // printf("red : %s\n", type_to_str(p->current_token->type));
        while (is_redirection(p->current_token) || is_type(p->current_token, TOK_IONUMBER))
        {
            
            struct node_redirection *r = NULL;
            if (parse_redirection(p, &r))
            {
                //free_redirection(r);
                return true;
            }
            new->redirections = append_redirection(new, r); //
            //(*ast)->redirections = append_redirection(*ast, r);
        }
        *ast = new; //
        return false;
    }
    new->type = SHELL_COMMAND;
    //(*ast)->type = SHELL_COMMAND;
    while (is_redirection(p->current_token) || is_type(p->current_token, TOK_IONUMBER))
    {
        struct node_redirection *r = NULL;
        if (parse_redirection(p, &r))
        {
            //free_redirection(r);
            return true;
        }
        new->redirections = append_redirection(new, r);
        //(*ast)->redirections = append_redirection(*ast, r);
    }
    *ast = new; //
    return false;
}

void parse_multiple_element(struct parser *parser, struct node_simple_command *ast)
{
    struct node_element *e = NULL;
    struct token *current = parser->current_token;
    while (!parse_element(parser, &e))
    {
        current = parser->current_token;
        append_element(ast, e);
        // printf("element : %s\n", e->element.word);
        e = NULL;
    }
    parser->current_token = current;
}

void parse_multiple_prefix(struct parser *parser, struct node_simple_command *ast)
{
    struct node_prefix *p = NULL;
    struct token *current = parser->current_token;
    while (!parse_prefix(parser, &p))
    {
        current = parser->current_token;
        append_prefix(ast, p);
        p = NULL;
    }
    parser->current_token = current;
}

bool parse_simple_command(struct parser *parser, struct node_simple_command **ast)
{
    DEBUG("parse_simple_command\n");
    struct token *current = parser->current_token;
    struct node_prefix *p = NULL;
    struct node_simple_command *new = build_simple_command();
    if (is_type(current, TOK_WORD) && is(current->value, "export"))
    {
        new->to_export = true;
        if (get_next_token(parser) && is_type(get_next_token(parser), TOK_WORD) && is(get_next_token(parser)->value, "-n"))
        {
            // printf("tiret n\n");
            next_token(parser);
            //next_token(parser);
            current = parser->current_token;
            parser->current_token = current;
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new); // export -n toto tata ....
        }
        else if (get_next_token(parser) && is_type(get_next_token(parser), TOK_WORD) && is(get_next_token(parser)->value, "-p"))
        {
            // printf("tiret p\n");
            next_token(parser);     //export -p
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new);
            //next_token(parser);
        }
        else if (get_next_token(parser) && (is_type(get_next_token(parser), TOK_WORD) || is_redirection(get_next_token(parser)) || is_type(get_next_token(parser), TOK_IONUMBER)))
        {
            // printf("export juste avec nom variable\n");
            next_token(parser);
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new);


        }
        else
        {
            // printf("export assignment word\n");
            next_token(parser);
            parse_multiple_prefix(parser, new); //export  a=b c=d ...
        }
    }
    else if (parse_prefix(parser, &p))
    {
        parser->current_token = current;
        struct node_element *e = NULL;
        if (parse_element(parser, &e))
            return true;
        append_element(new, e);
        parse_multiple_element(parser, new);
    }
    else
    {
        current = parser->current_token;
        new->prefixes = append_prefix(new, p);
        parse_multiple_prefix(parser, new);
        parse_multiple_element(parser, new);
    }
    *ast = new;
    return false;
}

bool parse_shell_command(struct parser *parser, struct node_shell_command **ast)
{
    DEBUG("parse_shell_command\n");
    struct token *current = parser->current_token;
    struct node_shell_command *new = build_shell_command(parser); //
    //*ast = build_shell_command(parser);
    if (is_type(current, TOK_LCURL) || is_type(current, TOK_LPAREN))
    {
        // (*ast)->type = (is_type(current, TOK_LCURL) ? C_BRACKETS : PARENTHESIS);
        // déjà fait dans build_shell_command
        next_token(parser);
        if (parse_compound_list(parser, &(new->shell.compound_list))) //&((*ast)->shell.compound_list)
        {
            //free_compound_list((*ast)->shell.compound_list);
            return true;
        }
        current = parser->current_token;
        next_token(parser);
        if (is_type(current, TOK_RCURL) || is_type(current, TOK_RPAREN))
        {
            *ast = new; //
            return false;
        }
        return true;
    }
    else
    {
        current = parser->current_token;
        new->shell_type = get_shell_command_type(current->type);//(*ast)->shell_type = get_shell_command_type(current->type);
        if (parse_rule_for(parser, &(new->shell.rule_for)))//&((*ast)->shell.rule_for)
        {
            parser->current_token = current;
            //free((*ast)->shell.rule_for);
            if (parse_rule_while(parser, &(new->shell.rule_while)))//&((*ast)->shell.rule_while)
            {
                //free_while((*ast)->shell.rule_while);
                parser->current_token = current;
                if (parse_rule_until(parser, &(new->shell.rule_until)))//&((*ast)->shell.rule_until)
                {
                    //free_until((*ast)->shell.rule_until);
                    parser->current_token = current;
                    if (parse_rule_case(parser, &(new->shell.rule_case)))//&((*ast)->shell.rule_case)
                    {
                        //free_case((*ast)->shell.rule_case);
                        parser->current_token = current;
                        // printf("+++ before if : %s\n", type_to_str(parser->current_token->type));
                        // printf("+++ before if next : %s\n", type_to_str(parser->current_token->next->type));
                        if (parse_rule_if(parser, &(new->shell.rule_if)))//&((*ast)->shell.rule_if)
                        {
                            //free_if((*ast)->shell.rule_if);
                            parser->current_token = current;
                            return true;
                        }
                    }
                }
            }
        }
    }
    *ast = new; //
    return false;
}

bool parse_funcdec(struct parser *parser, struct node_funcdec **ast)
{
    DEBUG("parse_funcdec\n");
    struct node_funcdec *new = build_funcdec();//*ast = build_funcdec();
    // printf("AMAZONIA\n");
    struct token *current = parser->current_token;
    bool is_function = false;
    if (strcmp("function", current->value) == 0)
    {
        is_function = true;
        next_token(parser);
    }
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    char *function_name = parser->current_token->value;
    new->function_name = function_name;
    new->is_function = is_function;
    //(*ast)->function_name = function_name;
    //(*ast)->is_function = is_function;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_LPAREN))
        return true;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_RPAREN))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (parse_shell_command(parser, &(new->shell_command)))//&((*ast)->shell_command)
    {
        //free_shell_command((*ast)->shell_command);
        return true;
    }
    *ast = new;
    return false;
}

bool parse_redirection(struct parser *parser, struct node_redirection **ast)
{
    DEBUG("parse_redirection\n");
    struct node_redirection *new = build_redirection(parser);//*ast = build_redirection(parser);
    // printf("curr tok in redirection : %s\n", type_to_str(parser->current_token->type));
    if (is_type(parser->current_token, TOK_IONUMBER))
        next_token(parser);
    struct token *curr = parser->current_token;
    new->type = curr->type;//(*ast)->type = curr->type;
    if (!is_redirection(curr))
        return true;
    next_token(parser);
    curr = parser->current_token;
    new->right = curr->value;//(*ast)->right = curr->value;
    next_token(parser);

    if (!(is_type(curr, TOK_WORD) /*|| is_type(curr, TOK_HEREDOC) */ ))
        return true;
    *ast = new; //
    return false;
}

bool parse_prefix(struct parser *parser, struct node_prefix **ast)
{
    DEBUG("parse_prefix\n");
    struct node_prefix *new = build_prefix(parser);//*ast = build_prefix(parser);
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        new->type = ASSIGMENT_WORD;
        new->prefix.assigment_word->variable_name = parser->current_token->value;
        //(*ast)->type = ASSIGMENT_WORD;
        //(*ast)->prefix.assigment_word->variable_name = parser->current_token->value;
        next_token(parser);
        new->prefix.assigment_word->value = parser->current_token->value;//(*ast)->prefix.assigment_word->value = parser->current_token->value;
        next_token(parser);
        *ast = new; //
        return false;
    }
    if (is_redirection(parser->current_token) ||is_type(parser->current_token, TOK_IONUMBER))
    {
        new->type = REDIRECTION;//(*ast)->type = REDIRECTION;
        if (parse_redirection(parser, &(new->prefix.redirection)))//&((*ast)->prefix.redirection)
        {
            //free_redirection((*ast)->prefix.redirection);
            return true;
        }
        *ast = new;//
        return false;
    }
    return true;
}

bool parse_element(struct parser *parser, struct node_element **ast)
{
    DEBUG("parse_element\n"); 
    //printf("curr tok = %s\n", type_to_str(parser->current_token->type));   
    struct node_element *new = build_element(parser);//*ast = build_element(parser);
    // if (is_type(parser->current_token, TOK_WORD) && strcmp(parser->current_token->value, "export") == 0){
    //     next_token(parser);

        // struct node_input *new2 = build_input();                    //EXPORT
        // if(parse_input(parser, new2))
        //     return true;                                 export [-f] [-n] [name[=value] ...]      // export -f function_name // export -n variable_name // export name=value
    //     return false;
    // }
    if (is_type(parser->current_token, TOK_WORD))
    {
        new->type = WORD;
        new->element.word = parser->current_token->value;
        //(*ast)->type = WORD;
        //(*ast)->element.word = parser->current_token->value;
        next_token(parser);
        *ast = new;
        return false;
    }
    if (is_redirection(parser->current_token) || is_type(parser->current_token, TOK_IONUMBER))
    {
        new->type = TOKEN_REDIRECTION;//(*ast)->type = TOKEN_REDIRECTION;
        if (parse_redirection(parser, &(new->element.redirection)))//&((*ast)->element.redirection)
        {
            //free_redirection((*ast)->element.redirection);
            return true;
        }
        *ast = new;
        return false;
    }
    return true;
}

bool parse_compound_list(struct parser *parser, struct node_compound_list **ast)
{
    DEBUG("parse_compound_list\n");
    parser_eat(parser);
    struct node_compound_list *new = build_compound_list();//*ast = build_compound_list();
    struct node_compound_list *tmp = new;
    //struct node_compound_list *tmp = *ast;
    if (parse_and_or(parser, &(tmp->and_or)))
    {
        //free_and_or(tmp->and_or);
        return true;
    }
    parser_comment(parser);
    if (is_type(parser->current_token, TOK_SEMI) ||
        is_type(parser->current_token, TOK_SEPAND) ||
        is_type(parser->current_token, TOK_NEWLINE)
        )
    {
        next_token(parser);
        parser_eat(parser);
        // tmp->next_sibling = build_compound_list();
        struct node_compound_list *node = build_compound_list();
        while (!parse_and_or(parser, &(node->and_or))) // &(tmp->next_sibling->and_or)
        {
            tmp->next_sibling = node;
            tmp = tmp->next_sibling;
            parser_comment(parser);
            if (!(is_type(parser->current_token, TOK_SEMI) ||
                is_type(parser->current_token, TOK_SEPAND) ||
                is_type(parser->current_token, TOK_NEWLINE)
                ))
            {
                return true;
            }
            next_token(parser);
            parser_eat(parser);
            // tmp->next_sibling = build_compound_list();
            node = build_compound_list();
        }
        //free_and_or(tmp->next_sibling->and_or);
        //free_compound_list(tmp->next_sibling);
        //tmp->next_sibling = NULL;
        parser_eat(parser);
        *ast = new; //
        return false;
    }
    return true;
    // while (is_type(parser->current_token, TOK_SEMI) ||
    //     is_type(parser->current_token, TOK_SEPAND) ||
    //     is_type(parser->current_token, TOK_NEWLINE)
    //     )
    // {
    //     // printf("line\n");
    //     next_token(parser);
    //     parser_eat(parser);
    //     tmp->next_sibling = build_compound_list();
        
    //     if (parse_and_or(parser, &(tmp->next_sibling->and_or)))
    //     {
    //         free_and_or(tmp->next_sibling->and_or);
    //         free_compound_list(tmp->next_sibling);
    //         tmp->next_sibling = NULL;
    //         parser_eat(parser);
    //         return false;
    //     }
    //     tmp = tmp->next_sibling;
    //     parser_comment(parser);
    //     //next_token(parser);
    //     //printf("parser.c / parse_CL : current_token : %d\n", parser->current_token->type);
    // }
    // return false;
}


bool parse_rule_for(struct parser *parser, struct node_for **ast)
{
    DEBUG("parse_rule_for\n");
    struct token *current = parser->current_token;
    struct node_for *new = build_for();//*ast = build_for();
    if (!(is_type(current, KW_FOR)))
    {
        //FREE(AST)
        return true;
    }
    next_token(parser);
    current = parser->current_token;
    if (!(is_type(current, TOK_WORD)))
    {
        //FREE(AST)
        return true;
    }
    new->variable_name = current->value;//(*ast)->variable_name = current->value;
    next_token(parser);
    // parser_eat(parser);
    if (is_type(parser->current_token, TOK_SEMI))
    {
        next_token(parser);
        current = parser->current_token;
        //FREE(AST)
    }
    else
    {
        parser_eat(parser);
        current = parser->current_token;
        if (!(is_type(current, KW_IN)))
        {
            //FREE(AST)
            return true;
        }
        next_token(parser);
        current = parser->current_token;
        while (is_type(current, TOK_WORD))
        {
            new->range = append_value_to_for(new, current->value);//(*ast)->range = append_value_to_for(*ast, current->value);
            next_token(parser);
            current = parser->current_token;
        }
        parser_comment(parser);
        if (!(is_type(current, TOK_SEMI) || is_type(current, TOK_NEWLINE)))
        {
            return true;
        }
        next_token(parser);
    }
    current = parser->current_token;
    parser_eat(parser);
    if (parse_do_group(parser, &(new->body))) //&((*ast)->body)
    {
        //free_do_group((*ast)->body);
        return true;
    }
    *ast = new;
    return false;
}

bool parse_rule_while(struct parser *parser, struct node_while **ast)
{
    DEBUG("parse_rule_while\n");
    struct token *current = parser->current_token;
    // *ast = build_while();
    struct node_while *new = build_while(); //
    if (is_type(current, KW_WHILE))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->condition))) // &((*ast)->condition)
        {
            // free_compound_list((*ast)->condition);
            return true;
        }
        if (parse_do_group(parser, &(new->body))) // &((*ast)->body)
        {
            // free_do_group((*ast)->body);
            return true;
        }
        *ast = new; //
        return false;
    }
    return true;
}

bool parse_rule_until(struct parser *parser, struct node_until **ast)
{
    DEBUG("parse_rule_until\n");
    struct token *current = parser->current_token;
    // *ast = build_until();
    struct node_until *new = build_until(); //
    if (is_type(current, KW_UNTIL))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->condition))) // &((*ast)->condition)
        {
            // free_compound_list((*ast)->condition);
            return true;
        }
        if (parse_do_group(parser, &(new->body))) // &((*ast)->body)
        {
            // free_do_group((*ast)->body);
            return true;
        }
        *ast = new; //
        return false;
    }
    return true;
}

bool parse_rule_case(struct parser *parser, struct node_case **ast)
{
    DEBUG("parse_rule_case\n");
    struct node_case *new = build_case(parser);//*ast = build_case(parser);
    if (!is_type(parser->current_token, KW_CASE))
        return true;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    new->word = parser->current_token->value;//(*ast)->word = parser->current_token->value;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_IN))
        return true;
    next_token(parser);
    parser_eat(parser);
    //next_token(parser);
    if (!is_type(parser->current_token, KW_ESAC))
    {
        if (parse_case_clause(parser, &(new->case_clause)))//&((*ast)->case_clause)
        {
            //printf("DAUPHIN");
            //free_case_clause((*ast)->case_clause);
            return true;
        }
        if (!is_type(parser->current_token, KW_ESAC))
            return true;
        new->is_case_clause = true;//(*ast)->is_case_clause = true;
    }
    next_token(parser);
    new->is_case_clause = true;
    *ast = new;
    return false;
}

bool parse_rule_if(struct parser *parser, struct node_if **ast)
{
    DEBUG("parse_rule_if\n");
    struct token *curr = parser->current_token;
    // *ast = build_if();
    struct node_if *new = build_if(); //
    // printf("in parse if curr token : %s\n", type_to_str(parser->current_token->type));
    if (!is_type(curr, KW_IF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->condition))) // &((*ast)->condition)
    {
        // free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;

    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->if_body))) // &((*ast)->if_body)
    {
        // free_compound_list((*ast)->if_body);
        return true;
    }
    curr = parser->current_token;
    // printf("Avant : parser.c / parse_if : current token = %s\n", type_to_str(curr->type));
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &(new->else_clause))) // &((*ast)->else_clause)
        {
            // free_else_clause((*ast)->else_clause);
            return true;
        }
        curr = parser->current_token;
    }
    // printf("Après : parser.c / parse_if : current token = %s\n", type_to_str(curr->type));
    if (!(is_type(curr, KW_FI)))
    {
        return true;
    }
    next_token(parser);
    *ast = new; //
    return false;
}

bool parse_rule_elif(struct parser *parser, struct node_if **ast)
{
    DEBUG("parse_rule_elif\n");
    struct token *curr = parser->current_token;
    struct node_if *new = build_if();//*ast = build_if();
    if (!is_type(curr, KW_ELIF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->condition)))//&((*ast)->condition)
    {
        //free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;
    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->if_body)))//&((*ast)->if_body)
    {
        //free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &(new->else_clause)))//&((*ast)->else_clause)
        {
            //free_else_clause((*ast)->else_clause);
            return true;
        }
        curr = parser->current_token;
    }
    *ast = new;
    return false;
}

bool parse_else_clause(struct parser *parser, struct node_else_clause **ast)
{
    DEBUG("parse_else_clause\n");
    struct token *current = parser->current_token;
    if (is_type(current, KW_ELSE))
    {
        next_token(parser);
        struct node_else_clause *new = build_else_clause(parser);//*ast = build_else_clause(parser);
        new->type = ELSE;//(*ast)->type = ELSE;
        if (parse_compound_list(parser, &(new->clause.else_body)))//&((*ast)->clause.else_body)
        {
            //free_compound_list((*ast)->clause.else_body);
            return true;
        }
        *ast = new;
        return false;
    }
    if (is_type(current, KW_ELIF))
    {
        struct node_else_clause *new = build_else_clause(parser);//*ast = build_else_clause(parser);
        new->type = ELIF;
        if (parse_rule_elif(parser, &(new->clause.elif)))//&((*ast)->clause.elif
        {
            //free_if((*ast)->clause.elif);
            return true;
        }
        *ast = new;
        return false;
    }
    return true;
}

bool parse_do_group(struct parser *parser, struct node_do_group **ast)
{
    DEBUG("parse_do_group\n");
    struct token *current = parser->current_token;
    if (is_type(current, KW_DO))
    {
        // *ast = build_do_group();
        struct node_do_group *new = build_do_group(); //
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->body))) // &((*ast)->body)
        {
            // free_compound_list((*ast)->body);
            //free_do_group(ast);
            return true;
        }
        current = parser->current_token;
        next_token(parser);
        if (!is_type(current, KW_DONE))
            return true;
        *ast = new;
        return false;
    }
    return true;
}

bool parse_case_clause(struct parser *parser, struct node_case_clause **ast)
{
    DEBUG("parse_case_clause\n");
    //struct token *current = parser->current_token;
    struct node_case_clause *new = build_case_clause();//*ast = build_case_clause();
    
    if (parse_case_item(parser, &(new->case_item)))//&((*ast)->case_item)
    {
        //free_case_item((*ast)->case_item);
        return true;
    }
    // printf("cas_clause : ct = %d\n", parser->current_token->type);
    struct node_case_clause *tmp = new;
    while (is_type(parser->current_token, KW_DSEMI) && !is_type(get_next_token(parser), KW_ESAC))
    {
        //créer node double semicol
        tmp->next = build_case_clause();
        tmp = tmp->next;
        next_token(parser);
        parser_eat(parser);
        if (parse_case_item(parser, &(tmp->case_item)))
        {
            // free_case_item(tmp->case_item);
            return true;
        }
        //next_token(parser);
    }
    
    if (is_type(parser->current_token, KW_DSEMI))
    {
        // break;
        //créer node semicolon
        // printf("zbl\n");
        next_token(parser);
    }
    parser_eat(parser);
    // printf("parse_CC : curr tok : %d\n", parser->current_token->type);
    *ast = new;
    return false;
}

bool parse_case_item(struct parser *parser, struct node_case_item **ast)
{
    DEBUG("parse_case_item\n");
    if (is_type(parser->current_token, TOK_LPAREN))
        next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    
    // *ast = build_case_item();
    struct node_case_item *new = build_case_item(); //
    // (*ast)->words = append_word_list((*ast), parser->current_token->value);
    new->words = append_word_list(new, parser->current_token->value);
    // (*ast)->type = NEXT;
    next_token(parser);
    // struct node_case_item *tmp = *ast;
    while (is_type(parser->current_token, TOK_PIPE))
    {
        // printf("oui\n");
        next_token(parser);
        // tmp->next.next = build_case_item();
        // tmp = tmp->next.next;
        // tmp->type = NEXT;
        if (!is_type(parser->current_token, TOK_WORD))
            return true;
        // printf("oui\n");
        // (*ast)->words = append_word_list((*ast), parser->current_token->value);
        new->words = append_word_list(new, parser->current_token->value);
        next_token(parser);
    }
    
    if (!is_type(parser->current_token, TOK_RPAREN))
        return true;
    
    next_token(parser);
    parser_eat(parser);
    
    //struct token *current = get_next_token(parser);
    if (!is_type(parser->current_token, KW_DSEMI) &&
        !is_type(parser->current_token, TOK_NEWLINE) &&
        !is_type(parser->current_token, KW_ESAC))
    {
        //(*ast)->type = COMPOUND;
        // printf("Current token type 3 : %d\n", parser->current_token->type);
        if (parse_compound_list(parser, &(new->compound_list))) // &((*ast)->compound_list)
        {
            // free_compound_list((*ast)->compound_list);
            return true;
        }
    }
    *ast = new; //
    //next_token(parser);
    return false;
}
