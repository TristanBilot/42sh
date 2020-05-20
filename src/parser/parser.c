#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../parser/parser.h"
#include "../utils/parser_utils.h"
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"
#include "../storage/program_data_storage.h"

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
        while (!is_type(p->current_token, TOK_NEWLINE)
            && !is_type(p->current_token, TOK_EOF))
            p->current_token = get_next_token(p);
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
    struct node_input *ast = NULL;
    if (!lexer)
        error("Struct error");
    struct parser *parser = init_parser(lexer);

    if (parse_input(parser, &ast))
    {
        error("Parser error");
        update_last_status(2);
    }
    return ast;
}

bool parse_input(struct parser *parser, struct node_input **ast)
{
    DEBUG("parse_input\n");
    if (!parser->lexer)
        return true;

    parser_comment(parser);
    struct node_input *node = build_input();
    if ((is_type(parser->current_token, TOK_NEWLINE)
        && is_type(parser->current_token->next, TOK_EOF))
        || is_type(parser->current_token, TOK_EOF))
    {
        *ast = node;
        return false;
    }
    if (!parse_list(parser, &(node->node_list)))
    {
        parser_comment(parser);
        if (is_type(parser->current_token, TOK_EOF)
            || (is_type(parser->current_token, TOK_NEWLINE)
            && is_type(parser->current_token->next, TOK_EOF)))
        {
            *ast = node;
            return false;
        }
    }
    return true;
}

bool parse_list(struct parser *parser, struct node_list **ast)
{
    DEBUG("parse_list\n");
    if (!parser)
        return true;
    struct node_list *node = build_list();
    struct node_list *tmp = node;
    bool is_first = true;
    do
    {
        if (is_first)
            is_first = !is_first;
        else
        {
            tmp->next_sibling = build_list();
            tmp = tmp->next_sibling;
        }
        if (parse_and_or(parser, &(tmp->and_or)))
            return true;
        parser_comment(parser);
        if (!is_type(parser->current_token, TOK_SEMI)
            && !is_type(parser->current_token, TOK_SEPAND)
            && !is_type(parser->current_token, TOK_NEWLINE)
            && !is_type(parser->current_token, TOK_EOF))
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
        else
            tmp->type = NONE;
        parser_comment(parser);
    } while (!is_type(parser->current_token, TOK_NEWLINE)
        && !is_type(parser->current_token, TOK_EOF));
    *ast = node;
    return false;
}

bool parse_and_or(struct parser *parser, struct node_and_or **ast)
{
    DEBUG("parse_and_or\n");
    if (!parser)
        return true;
    struct node_and_or *new = build_and_or_final(false, NULL, NULL);
    if (parse_pipeline(parser, &(new->left.pipeline)))
        return true;
    int count = 0;
    while (is_type(parser->current_token, TOK_AND) ||
        is_type(parser->current_token, TOK_OR))
    {
        bool is_and = is_type(parser->current_token, TOK_AND);
        next_token(parser);
        parser_eat(parser);
        if (parse_pipeline(parser, &(new->right)))
            return true;
        if (count == 0)
            new->left.and_or = build_and_or_final(is_and, new->left.pipeline,
                new->right);
        else
            new->left.and_or = build_and_or_merge(is_and, new->left.and_or,
                new->right);
        count++;
    }
    if (count <= 0)
        *ast = new;
    else
        *ast = new->left.and_or;
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
    struct node_pipeline *new = build_pipeline(is_not);
    struct node_pipeline *tmp = new;
    if (!parse_command(parser, &(new->command)))
    {
        while (is_type(parser->current_token, TOK_PIPE))
        {
            tmp->next_sibling = build_pipeline(false);
            tmp = tmp->next_sibling;
            next_token(parser);
            parser_eat(parser);
            if (parse_command(parser, &(tmp->command)))
                return true;
        }
        *ast = new;
        return false;
    }
    return true;
}

bool parse_command(struct parser *p, struct node_command **ast)
{
    DEBUG("parse_command\n");
    struct token *current = p->current_token;
    struct node_command *new = build_command();
    if (parse_shell_command(p, &(new->command.shell_command)))
    {
        p->current_token = current;
        if (parse_funcdec(p, &(new->command.funcdec)))
        {
            p->current_token = current;
            if (parse_simple_command(p, &(new->command.simple_command)))
                return true;
            new->type = SIMPLE_COMMAND;
            *ast = new;
            return false;
        }
        new->type = FUNCDEC;
        while (is_redirection(p->current_token)
            || is_type(p->current_token, TOK_IONUMBER))
        {
            struct node_redirection *r = NULL;
            if (parse_redirection(p, &r))
                return true;
            new->redirections = append_redirection(new, r);
        }
        *ast = new;
        return false;
    }
    new->type = SHELL_COMMAND;
    while (is_redirection(p->current_token)
        || is_type(p->current_token, TOK_IONUMBER))
    {
        struct node_redirection *r = NULL;
        if (parse_redirection(p, &r))
            return true;
        new->redirections = append_redirection(new, r);
    }
    *ast = new;
    return false;
}

void parse_multiple_element(struct parser *parser,
    struct node_simple_command *ast)
{
    struct node_element *e = NULL;
    struct token *current = parser->current_token;
    while (!parse_element(parser, &e))
    {
        current = parser->current_token;
        append_element(ast, e);
        e = NULL;
    }
    parser->current_token = current;
}

void parse_multiple_prefix(struct parser *parser,
    struct node_simple_command *ast)
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

bool parse_simple_command(struct parser *parser,
    struct node_simple_command **ast)
{
    DEBUG("parse_simple_command\n");
    struct token *current = parser->current_token;
    struct node_prefix *p = NULL;
    struct node_simple_command *new = build_simple_command();
    if (is_type(current, TOK_WORD) && (is(current->value, "export")
        || is(current->value, "alias")))
    {
        if (is(current->value, "export"))
            new->to_export = true;
        if (is(current->value, "alias"))
            new->to_alias = true;
        if (get_next_token(parser)
            && is_type(get_next_token(parser), TOK_WORD)
            && is(get_next_token(parser)->value, "-n"))
        {
            next_token(parser);
            current = parser->current_token;
            parser->current_token = current;
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new); // export -n toto tata ....
        }
        else if (get_next_token(parser)
            && is_type(get_next_token(parser), TOK_WORD)
            && is(get_next_token(parser)->value, "-p"))
        {
            next_token(parser);     //export -p
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new);
        }
        else if (get_next_token(parser)
            && (is_type(get_next_token(parser), TOK_WORD)
            || is_redirection(get_next_token(parser))
            || is_type(get_next_token(parser), TOK_IONUMBER)))
        {
            next_token(parser);
            struct node_element *e = NULL;
            if (parse_element(parser, &e))
                return true;
            append_element(new, e);
            parse_multiple_element(parser, new);
        }
        else
        {
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

bool parse_shell_command(struct parser *parser,
    struct node_shell_command **ast)
{
    DEBUG("parse_shell_command\n");
    struct token *current = parser->current_token;
    struct node_shell_command *new = build_shell_command(parser);
    if (is_type(current, TOK_LCURL) || is_type(current, TOK_LPAREN))
    {
        next_token(parser);
        if (parse_compound_list(parser, &(new->shell.compound_list)))
            return true;
        current = parser->current_token;
        next_token(parser);
        if (is_type(current, TOK_RCURL) || is_type(current, TOK_RPAREN))
        {
            *ast = new;
            return false;
        }
        return true;
    }
    else
    {
        current = parser->current_token;
        new->shell_type = get_shell_command_type(current->type);
        if (parse_rule_for(parser, &(new->shell.rule_for)))
        {
            parser->current_token = current;
            if (parse_rule_while(parser, &(new->shell.rule_while)))
            {
                parser->current_token = current;
                if (parse_rule_until(parser, &(new->shell.rule_until)))
                {
                    parser->current_token = current;
                    if (parse_rule_case(parser, &(new->shell.rule_case)))
                    {
                        parser->current_token = current;
                        if (parse_rule_if(parser, &(new->shell.rule_if)))
                        {
                            parser->current_token = current;
                            return true;
                        }
                    }
                }
            }
        }
    }
    *ast = new;
    return false;
}

bool parse_funcdec(struct parser *parser, struct node_funcdec **ast)
{
    DEBUG("parse_funcdec\n");
    struct node_funcdec *new = build_funcdec();
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
    next_token(parser);
    if (!is_type(parser->current_token, TOK_LPAREN))
        return true;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_RPAREN))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (parse_shell_command(parser, &(new->shell_command)))
        return true;
    *ast = new;
    return false;
}

bool parse_redirection(struct parser *parser, struct node_redirection **ast)
{
    DEBUG("parse_redirection\n");
    struct node_redirection *new = build_redirection(parser);
    if (is_type(parser->current_token, TOK_IONUMBER))
        next_token(parser);
    struct token *curr = parser->current_token;
    new->type = curr->type;
    if (!is_redirection(curr))
        return true;
    next_token(parser);
    curr = parser->current_token;
    new->right = curr->value;
    next_token(parser);

    if (!(is_type(curr, TOK_WORD) /*|| is_type(curr, TOK_HEREDOC) */ ))
        return true;
    *ast = new;
    return false;
}

bool parse_prefix(struct parser *parser, struct node_prefix **ast)
{
    DEBUG("parse_prefix\n");
    struct node_prefix *new = build_prefix(parser);
    if (is_type(parser->current_token, TOK_ASS_WORD))
    {
        new->type = ASSIGMENT_WORD;
        new->prefix.assigment_word->variable_name = \
            parser->current_token->value;
        next_token(parser);
        new->prefix.assigment_word->value = parser->current_token->value;
        next_token(parser);
        *ast = new;
        return false;
    }
    if (is_redirection(parser->current_token)
        || is_type(parser->current_token, TOK_IONUMBER))
    {
        new->type = REDIRECTION;
        if (parse_redirection(parser, &(new->prefix.redirection)))
            return true;
        *ast = new;
        return false;
    }
    return true;
}

bool parse_element(struct parser *parser, struct node_element **ast)
{
    DEBUG("parse_element\n");
    struct node_element *new = build_element(parser);
    if (is_type(parser->current_token, TOK_WORD))
    {
        new->type = WORD;
        new->element.word = parser->current_token->value;
        next_token(parser);
        *ast = new;
        return false;
    }
    if (is_redirection(parser->current_token)
        || is_type(parser->current_token, TOK_IONUMBER))
    {
        new->type = TOKEN_REDIRECTION;
        if (parse_redirection(parser, &(new->element.redirection)))
            return true;
        *ast = new;
        return false;
    }
    return true;
}

bool parse_compound_list(struct parser *parser,
    struct node_compound_list **ast)
{
    DEBUG("parse_compound_list\n");
    parser_eat(parser);
    struct node_compound_list *new = build_compound_list();
    struct node_compound_list *tmp = new;
    if (parse_and_or(parser, &(tmp->and_or)))
        return true;
    parser_comment(parser);
    if (is_type(parser->current_token, TOK_SEMI)
        || is_type(parser->current_token, TOK_SEPAND)
        || is_type(parser->current_token, TOK_NEWLINE))
    {
        next_token(parser);
        parser_eat(parser);
        struct node_compound_list *node = build_compound_list();
        while (!parse_and_or(parser, &(node->and_or)))
        {
            tmp->next_sibling = node;
            tmp = tmp->next_sibling;
            parser_comment(parser);
            if (!(is_type(parser->current_token, TOK_SEMI)
                || is_type(parser->current_token, TOK_SEPAND)
                || is_type(parser->current_token, TOK_NEWLINE)))
                return true;
            next_token(parser);
            parser_eat(parser);
            node = build_compound_list();
        }
        parser_eat(parser);
        *ast = new;
        return false;
    }
    return true;
}

bool parse_rule_for(struct parser *parser, struct node_for **ast)
{
    DEBUG("parse_rule_for\n");
    struct token *current = parser->current_token;
    struct node_for *new = build_for();
    if (!(is_type(current, KW_FOR)))
        return true;
    next_token(parser);
    current = parser->current_token;
    if (!(is_type(current, TOK_WORD)))
        return true;
    new->variable_name = current->value;
    next_token(parser);
    if (is_type(parser->current_token, TOK_SEMI))
    {
        next_token(parser);
        current = parser->current_token;
    }
    else
    {
        parser_eat(parser);
        current = parser->current_token;
        if (!(is_type(current, KW_IN)))
            return true;
        next_token(parser);
        current = parser->current_token;
        while (is_type(current, TOK_WORD))
        {
            new->range = append_value_to_for(new, current->value);
            next_token(parser);
            current = parser->current_token;
        }
        parser_comment(parser);
        if (!(is_type(current, TOK_SEMI) || is_type(current, TOK_NEWLINE)))
            return true;
        next_token(parser);
    }
    current = parser->current_token;
    parser_eat(parser);
    if (parse_do_group(parser, &(new->body)))
        return true;
    *ast = new;
    return false;
}

bool parse_rule_while(struct parser *parser, struct node_while **ast)
{
    DEBUG("parse_rule_while\n");
    struct token *current = parser->current_token;
    struct node_while *new = build_while();
    if (is_type(current, KW_WHILE))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->condition)))
            return true;
        if (parse_do_group(parser, &(new->body)))
            return true;
        *ast = new;
        return false;
    }
    return true;
}

bool parse_rule_until(struct parser *parser, struct node_until **ast)
{
    DEBUG("parse_rule_until\n");
    struct token *current = parser->current_token;
    struct node_until *new = build_until();
    if (is_type(current, KW_UNTIL))
    {
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->condition)))
            return true;
        if (parse_do_group(parser, &(new->body)))
            return true;
        *ast = new;
        return false;
    }
    return true;
}

bool parse_rule_case(struct parser *parser, struct node_case **ast)
{
    DEBUG("parse_rule_case\n");
    struct node_case *new = build_case(parser);
    if (!is_type(parser->current_token, KW_CASE))
        return true;
    next_token(parser);
    if (!is_type(parser->current_token, TOK_WORD))
        return true;
    new->word = parser->current_token->value;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_IN))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_ESAC))
    {
        if (parse_case_clause(parser, &(new->case_clause)))
            return true;
        if (!is_type(parser->current_token, KW_ESAC))
            return true;
        new->is_case_clause = true;
    }
    next_token(parser);
    new->is_case_clause = true;
    *ast = new;
    return false;
}

bool parse_rule_if(struct parser *parser, struct node_if **ast)
{
    struct token *curr = parser->current_token;
    struct node_if *new = build_if();
    if (!is_type(curr, KW_IF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->condition)))
        return true;
    curr = parser->current_token;

    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->if_body)))
        return true;
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &(new->else_clause)))
            return true;
        curr = parser->current_token;
    }
    if (!(is_type(curr, KW_FI)))
        return true;
    next_token(parser);
    *ast = new;
    return false;
}

bool parse_rule_elif(struct parser *parser, struct node_if **ast)
{
    DEBUG("parse_rule_elif\n");
    struct token *curr = parser->current_token;
    struct node_if *new = build_if();
    if (!is_type(curr, KW_ELIF))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->condition)))
        return true;
    curr = parser->current_token;
    if (!(is_type(curr, KW_THEN)))
        return true;
    next_token(parser);
    if (parse_compound_list(parser, &(new->if_body)))
        return true;
    curr = parser->current_token;
    if (is_type(curr, KW_ELSE) || is_type(curr, KW_ELIF))
    {
        if (parse_else_clause(parser, &(new->else_clause)))
            return true;
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
        struct node_else_clause *new = build_else_clause(parser);
        new->type = ELSE;
        if (parse_compound_list(parser, &(new->clause.else_body)))
            return true;
        *ast = new;
        return false;
    }
    if (is_type(current, KW_ELIF))
    {
        struct node_else_clause *new = build_else_clause(parser);
        new->type = ELIF;
        if (parse_rule_elif(parser, &(new->clause.elif)))
            return true;
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
        struct node_do_group *new = build_do_group();
        next_token(parser);
        current = parser->current_token;
        if (parse_compound_list(parser, &(new->body)))
            return true;
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
    struct node_case_clause *new = build_case_clause();
    if (parse_case_item(parser, &(new->case_item)))
        return true;
    struct node_case_clause *tmp = new;
    while (is_type(parser->current_token, KW_DSEMI)
        && !is_type(get_next_token(parser), KW_ESAC))
    {
        tmp->next = build_case_clause();
        tmp = tmp->next;
        next_token(parser);
        parser_eat(parser);
        if (parse_case_item(parser, &(tmp->case_item)))
            return true;
    }
    if (is_type(parser->current_token, KW_DSEMI))
        next_token(parser);
    parser_eat(parser);
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
    struct node_case_item *new = build_case_item();
    new->words = append_word_list(new, parser->current_token->value);
    next_token(parser);
    while (is_type(parser->current_token, TOK_PIPE))
    {
        next_token(parser);
        if (!is_type(parser->current_token, TOK_WORD))
            return true;
        new->words = append_word_list(new, parser->current_token->value);
        next_token(parser);
    }
    if (!is_type(parser->current_token, TOK_RPAREN))
        return true;
    next_token(parser);
    parser_eat(parser);
    if (!is_type(parser->current_token, KW_DSEMI)
        && !is_type(parser->current_token, TOK_NEWLINE)
        && !is_type(parser->current_token, KW_ESAC))
        if (parse_compound_list(parser, &(new->compound_list)))
            return true;
    *ast = new;
    return false;
}
