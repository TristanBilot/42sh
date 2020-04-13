/*#include "parser/parser.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/string.h"
#include "ast/ast.h"
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "ast/free.h"
#include "ast/ast.h"
#include "utils/parser_utils.h"
#include "utils/xalloc.h"
#include "utils/string.h"

#define DEBUG_FLAG true
#define DEBUG(msg) if (DEBUG_FLAG) \
                        printf("%s", msg);

struct parser *init_parser(struct lexer *lexer)
{
    DEBUG("init_parser\n");
    struct parser *parser = malloc(sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = peek(lexer);
    return parser;
}

void free_parser(struct parser *p)
{
    if(p)
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

void parser_eat(struct parser *p)
{
    DEBUG("parser_eat\n");
    while (is_type(p->current_token, TOK_NEWLINE))
        p->current_token = get_next_token(p);
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
    }
    free_parser(parser);
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
    if (is_type(parser->current_token, TOK_NEWLINE) ||
        is_type(parser->current_token, TOK_EOF))
    {
        return false;
    }
    *ast = build_input();
    if (!parse_list(parser, &((*ast)->node_list)))
        if (is_type(parser->current_token, TOK_EOF) ||
            is_type(parser->current_token, TOK_NEWLINE))
        {
            return false;
        }
    free_list((*ast)->node_list);
    return true;
}

bool parse_list(struct parser *parser, struct node_list **ast)
{
    DEBUG("parse_list\n");
    if (!parser)
        return true;
    struct node_list *tmp = *ast;
    do
    {
        tmp = build_list();
        if (parse_and_or(parser, &(tmp->and_or)))
        {
            free_and_or(tmp->and_or);
            return true;
        }
        if (!is_type(parser->current_token, TOK_SEMI) && 
            !is_type(parser->current_token, TOK_SEPAND) &&
            !is_type(parser->current_token, TOK_NEWLINE) && 
            !is_type(parser->current_token, TOK_EOF))
            return true;
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
        tmp = tmp->next_sibling;
    } while(!is_type(parser->current_token, TOK_NEWLINE) && 
        !is_type(parser->current_token, TOK_EOF));
    return false;
}

bool parse_and_or(struct parser *parser, struct node_and_or **ast)
{
    DEBUG("parse_and_or\n");
    if (!parser)
        return true;
    struct node_and_or *node_and_or = build_and_or_final(false, NULL, NULL);             // and_or: pipeline (('&&'|'||') ('\n')* pipeline)*
    
    if (parse_pipeline(parser, &(node_and_or->left.pipeline)))
    {
        free_pipeline(node_and_or->left.pipeline);
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
            free_pipeline(node_and_or->right);
            return true;
        }
        if (count == 0)
        {
            // free left.pipeline
            node_and_or->left.and_or = build_and_or_final(is_and, node_and_or->left.pipeline, node_and_or->right);
        }
        else
            node_and_or->left.and_or = build_and_or_merge(is_and, node_and_or->left.and_or, node_and_or->right);
        count++;
    }
    if (count <= 1)
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
    *ast = build_pipeline(is_not);
    struct node_pipeline *tmp = *ast;
    if (!parse_command(parser, &((*ast)->command)))
    {
        while (is_type(parser->current_token, TOK_PIPE))
        {
            tmp = tmp->next_sibling;
            next_token(parser);
            parser_eat(parser);
            tmp = build_pipeline(false);
            if (parse_command(parser, &(tmp->command)))
                return true;
        }
        return false;
    }
    return true;
}

bool parse_command(struct parser *p, struct node_command **ast)
{
    DEBUG("parse_command\n");
    struct token *current = p->current_token;
    *ast = build_command();
    if (parse_shell_command(p, &((*ast)->command.shell_command)))
    {
        //free_shell_command((*ast)->command.shell_command);
        if (parse_funcdec(p, &((*ast)->command.funcdec)))
        {
            // FREE ast->command.funcdec
            (*ast)->type = SIMPLE_COMMAND;
            p->current_token = current;
            return parse_simple_command(p, &((*ast)->command.simple_command));
        }
        while (is_redirection(p->current_token))
        {
            struct node_redirection *r = NULL;
            if (parse_redirection(p, &r))
                return true;
            (*ast)->redirections = append_redirection(*ast, r);
        }
    }
    (*ast)->type = SHELL_COMMAND;
    while (is_redirection(p->current_token))
    {
        struct node_redirection *r = NULL;
        if (parse_redirection(p, &r))
            return true;
        (*ast)->redirections = append_redirection(*ast, r);
    }
    return false;
}

bool parse_simple_command(struct parser *parser, struct node_simple_command **ast)
{
    DEBUG("parse_simple_command\n");
    struct token *current = parser->current_token;
    struct node_prefix *p = NULL;
    *ast = build_simple_command();
    if (parse_prefix(parser, &p))
    {
        // FREE p
        parser->current_token = current;
        struct node_element *e = NULL;
        if (parse_element(parser, &e))
        {
            free_element(e);
            return true;
        }
        (*ast)->elements = append_element(*ast, e);
        e = NULL;
        current = parser->current_token;
        while (!parse_element(parser, &e))
        {
            
            current = parser->current_token;
            append_element(*ast, e); // rajouter (*ast)->elements = ?
            e = NULL;
        }
        // FREE e
        parser->current_token = current;
    }
    else
    {
        current = parser->current_token;
        (*ast)->prefixes = append_prefix(*ast, p);
        p = NULL;
        while (!parse_prefix(parser, &p))
        {
            current = parser->current_token;
            (*ast)->prefixes = append_prefix(*ast, p);
            p = NULL;
        }
        // FREE p
        parser->current_token = current;
        struct node_element *e = NULL;
        while (!parse_element(parser, &e))
        {
            current = parser->current_token;
            (*ast)->elements = append_element(*ast, e);
            e = NULL;
        }
        // FREE e
        parser->current_token = current;
    }
    return false;
}

bool parse_shell_command(struct parser *parser, struct node_shell_command **ast)
{
    DEBUG("parse_shell_command\n");
    struct token *current = parser->current_token;
    *ast = build_shell_command(parser);
    if (is_type(current, TOK_LCURL) || is_type(current, TOK_LPAREN))
    {
        (*ast)->type = (is_type(current, TOK_LCURL) ? C_BRACKETS : PARENTHESIS);
        next_token(parser);
        if (parse_compound_list(parser, &((*ast)->shell.compound_list)))
            return true;
        current = parser->current_token;
        if (is_type(current, TOK_RCURL) || is_type(current, TOK_RPAREN))
            return false;
        return true;
    }
    else if (!parse_rule_for(parser, &((*ast)->shell.rule_for)))
    {
        (*ast)->type = RULE;
        return false;
    }
    free_for((*ast)->shell.rule_for);
    if (!parse_rule_while(parser, &((*ast)->shell.rule_while)))
    {
        (*ast)->type = RULE;
        return false;
    }
    free_while((*ast)->shell.rule_while);
    if (!parse_rule_until(parser, &((*ast)->shell.rule_until)))
    {
        (*ast)->type = RULE;
        return false;
    }
    free_until((*ast)->shell.rule_until);
    if (!parse_rule_case(parser, &((*ast)->shell.rule_case)))
    {
        (*ast)->type = RULE;
        return false;
    }
    free_case((*ast)->shell.rule_case);
    if (!parse_rule_if(parser, &((*ast)->shell.rule_if)))
    {
        (*ast)->type = RULE;
        return false;
    }
    free_if((*ast)->shell.rule_if);
    return true;
}

bool parse_funcdec(struct parser *parser, struct node_funcdec **ast)
{
    DEBUG("parse_funcdec\n");
    printf("AMAZONIA\n");
    struct token *current = parser->current_token;
    bool is_function = false;
    if (strcmp("function", current->value) == 0)
    {
        is_function = true;
        next_token(parser);
    }
    
    if (!is_type(current, TOK_WORD))
        return true;
    
    char *function_name = current->value;
    next_token(parser);
    if (!is_type(current, TOK_LPAREN))
        return true;
    next_token(parser);
    if (!is_type(current, TOK_RPAREN))
        return true;
    parser_eat(parser);
    *ast = build_funcdec(is_function, function_name);
    if (parse_shell_command(parser, &((*ast)->shell_command)))
    {
        free_shell_command((*ast)->shell_command);
        return true;
    }
    return false;
}

bool parse_redirection(struct parser *parser, struct node_redirection **ast)
{
    DEBUG("parse_redirection\n");
    *ast = build_redirection(parser);
    if (is_type(parser->current_token, TOK_IONUMBER))
        next_token(parser);
    struct token *curr = parser->current_token;
    (*ast)->type = curr->type;
    if (!is_redirection(curr))
        return true;
    next_token(parser);
    curr = parser->current_token;
    (*ast)->right = curr->value;
    next_token(parser);

    if (!(is_type(curr, TOK_WORD) /*|| is_type(curr, TOK_HEREDOC) */ ))
        return true;
    return false;
}

bool parse_prefix(struct parser *parser, struct node_prefix **ast)
{
    DEBUG("parse_prefix\n");
    *ast = build_prefix(parser);
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        (*ast)->type = ASSIGMENT_WORD;
        (*ast)->prefix.assigment_word.variable_name = parser->current_token->value;
        next_token(parser);
        (*ast)->prefix.assigment_word.value = parser->current_token->value;
        next_token(parser);
        return false;
    }
    if (is_type(parser->current_token, TOK_IONUMBER))
    {
        (*ast)->type = REDIRECTION;
        if (parse_redirection(parser, &((*ast)->prefix.redirection)))
        {
            free_redirection((*ast)->prefix.redirection);
            return true;
        }
        return false;
    }
    return true;
}

bool parse_element(struct parser *parser, struct node_element **ast)
{
    DEBUG("parse_element\n");    
    *ast = build_element(parser);
    if (is_type(parser->current_token, TOK_WORD))
    {
        (*ast)->type = WORD;
        (*ast)->element.word = parser->current_token->value;
        next_token(parser);
        return false;
    }
    if (is_redirection(parser->current_token))
    {
        (*ast)->type = TOKEN_REDIRECTION;
        if (parse_redirection(parser, &((*ast)->element.redirection)))
        {
            free_redirection((*ast)->element.redirection);
            return true;
        }
        return false;
    }
    return true;
}

bool parse_compound_list(struct parser *parser, struct node_compound_list **ast)
{
    DEBUG("parse_compound_list\n");
    parser_eat(parser);
    *ast = build_compound_list();
    struct node_compound_list *tmp = *ast;
    if (parse_and_or(parser, &(tmp->and_or)))
    {
        free_and_or(tmp->and_or);
        return true;
    }
    while (is_type(parser->current_token, TOK_SEMI) ||
        is_type(parser->current_token, TOK_SEPAND) ||
        is_type(parser->current_token, TOK_NEWLINE)
        )
    {
        parser_eat(parser);
        tmp = tmp->next_sibling;
        tmp = build_compound_list();
        if (parse_and_or(parser, &(tmp->and_or)))
        {
            parser_eat(parser);
            return false;
        }
    }
    return false;
}

bool parse_rule_for(struct parser *parser, struct node_for **ast) // PROBLEME REVOIR GRAMMAIRE
{
    DEBUG("parse_rule_for\n");
    struct token *current = parser->current_token;
    *ast = build_for();
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
    (*ast)->variable_name = current->value;
    next_token(parser);
    // parser_eat(parser);
    if (is_type(current, TOK_SEMI))
    {
        next_token(parser);
        current = parser->current_token;
        //FREE(AST)
    }
    else
        parser_eat(parser);
    
    if (!(is_type(current, KW_IN)))
    {
        //FREE(AST)
        return true;
    }
    next_token(parser);
    current = parser->current_token;
    while (is_type(current, TOK_WORD))
    {
        (*ast)->range = append_value_to_for(*ast, current->value);
        next_token(parser);
        current = parser->current_token;
    }
    if (!(is_type(current, TOK_SEMI) || is_type(current, TOK_NEWLINE)))
    {
        //FREE(AST)
        return true;
    }
    next_token(parser);
    current = parser->current_token;
    parser_eat(parser);
    return parse_do_group(parser, &((*ast)->body)) ? true : false;
}

bool parse_rule_while(struct parser *parser, struct node_while **ast)
{
    DEBUG("parse_rule_while\n");
    struct token *current = parser->current_token;
    *ast = build_while();
    if (is_type(current, KW_WHILE))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &((*ast)->condition)))
            return true;
        return parse_do_group(parser, &((*ast)->body));
    }
    return true;
}

bool parse_rule_until(struct parser *parser, struct node_until **ast)
{
    DEBUG("parse_rule_until\n");
    struct token *current = parser->current_token;
    *ast = build_until();
    if (is_type(current, KW_UNTIL))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &((*ast)->condition)))
            return true;
        if (parse_do_group(parser, &((*ast)->body)))
            return true;
        else
            return false;
    }
    return true;
}

bool parse_rule_case(struct parser *parser, struct node_case **ast)
{
    DEBUG("parse_rule_case\n");
    *ast = build_case(parser);
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
        if (parse_case_clause(parser, &((*ast)->case_clause)))
        {
            free_case_clause((*ast)->case_clause);
            return true;
        }
        if (!is_type(parser->current_token, KW_ESAC))
            return true;
    }
    next_token(parser);
    return false;
}

bool parse_rule_if(struct parser *parser, struct node_if **ast)
{
    DEBUG("parse_rule_if\n");
    struct token *curr = parser->current_token;
    *ast = build_if();
    if (!is_type(curr, KW_IF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &((*ast)->condition)))
    {
        free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;

    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &((*ast)->if_body)))
    {
        free_compound_list((*ast)->if_body);
        return true;
    }
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &((*ast)->else_clause)))
        {
            free_else_clause((*ast)->else_clause);
            return true;
        }
        curr = parser->current_token;
    }
    next_token(parser);
    if (!(is_type(curr, KW_FI)))
        return true;
    return false;
}

bool parse_rule_elif(struct parser *parser, struct node_if **ast)
{
    DEBUG("parse_rule_elif\n");
    struct token *curr = parser->current_token;
    *ast = build_if();
    if (!is_type(curr, KW_ELIF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &((*ast)->condition)))
    {
        free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;
    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &((*ast)->if_body)))
    {
        free_compound_list((*ast)->condition);
        return true;
    }
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &((*ast)->else_clause)))
        {
            free_else_clause((*ast)->else_clause);
            return true;
        }
        curr = parser->current_token;
    }
    return false;
}

bool parse_else_clause(struct parser *parser, struct node_else_clause **ast)
{
    DEBUG("parse_else_clause\n");
    struct token *current = parser->current_token;
    if (is_type(current, KW_ELSE))
    {
        next_token(parser);
        *ast = build_else_clause(parser);
        (*ast)->type = ELSE;
        if (parse_compound_list(parser, &((*ast)->clause.else_body)))
        {
            free_compound_list((*ast)->clause.else_body);
            return true;
        }
    }
    if (is_type(current, KW_ELIF))
    {
        *ast = build_else_clause(parser);
        (*ast)->type = ELIF;
        if (parse_rule_elif(parser, &((*ast)->clause.elif)))
        {
            free_if((*ast)->clause.elif);
            return true;
        }
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
        *ast = build_do_group();
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &((*ast)->body)))
        {
            free_compound_list((*ast)->body);
            return true;
        }
        return is_type(parser->current_token, KW_DONE);
    }
    return true;
}

bool parse_case_clause(struct parser *parser, struct node_case_clause **ast)
{
    DEBUG("parse_case_clause\n");
    struct token *current = parser->current_token;
    *ast = build_case_clause();
    if (parse_case_item(parser, &((*ast)->case_item)))
    {
        free_case_item((*ast)->case_item);
        return true;
    }
    next_token(parser);
    while (is_type(current, KW_DSEMI))
    {
        //créer node double semicol
        parser_eat(parser);
        *ast = (*ast)->next;
        *ast = build_case_clause();
        if (parse_case_item(parser, &((*ast)->case_item)))
        {
            free_case_item((*ast)->case_item);
            return true;
        }
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

bool parse_case_item(struct parser *parser, struct node_case_item **ast)
{
    DEBUG("parse_case_item\n");
    if (is_type(parser->current_token, TOK_LPAREN))
        next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    *ast = build_case_item(parser);
    next_token(parser);
    while (is_type(parser->current_token, TOK_PIPE))
    {
        next_token(parser);
        (*ast)->type = NEXT;
        *ast = (*ast)->next.next;
        if (!is_type(parser->current_token, TOK_WORD))
            return true;
        (*ast)->word = parser->current_token->value;
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
        (*ast)->type = COMPOUND;
        if (parse_compound_list(parser, &((*ast)->next.compound_list)))
        {
            free_compound_list((*ast)->next.compound_list);
            return true;
        }
    }
    next_token(parser);
    return false;
}
