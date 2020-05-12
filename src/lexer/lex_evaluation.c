#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/string_utils.h"
#include "../utils/index_utils.h"
#include "../lexer/lexer.h"
#include "../utils/xalloc.h"

char *lex_backslash(char *c, size_t i)
{
    if (!c || !c[i] || c[i] != '\\')
        return NULL;

    char next = c[i + 1];
    if (next == EOF)
        (void) c; /* TODO (echo \ \\\) */

    if (next && next != '\\')
        return substr(c, i, strlen(c));
        
    if (next && next == '\\')
        return "\\"; /* NEED IMPROVEMENTS LATER */
    return NULL;
}

struct token *lex_great_less_and(const char *c, size_t i)
{
    if (!c || !c[i] || !c[i + 1])
        return NULL;
    if (c[i] == '<' && c[i + 1] == '&') /* <& */
        return new_token_type(TOK_LESSAND);
    if (c[i] == '>' && c[i + 1] == '&') /* >& */
        return new_token_type(TOK_GREATAND);
    return NULL;
}

struct token *lex_io_number(char *c, size_t *i)
{
    if (!c || !c[*i] || !c[*i+1])
        return NULL;
    
    if (is_number(c[*i]) && ((*i == 0) || ((c[*i-1] && (c[*i-1] == ' ')) || !c[*i-1]))) /* 2>a */
        if (c[*i+1] == '<' || c[*i+1] == '>')
            return new_token_io_number(c[*i]);
    return NULL;
}

struct token *lex_great_less(char *c, size_t i)
{
    if (!c || !c[i])
        return NULL;
    if (c[i + 1])
    {
        if (c[i] == '<' && c[i + 1] == '<' && c[i + 2] && c[i + 2] == '-')
            return new_token_type(TOK_DLESSDASH);
        if (c[i] == '<' && c[i + 1] == '<')
            return new_token_type(TOK_DLESS);
        if (c[i] == '>' && c[i + 1] == '>')
            return new_token_type(TOK_DGREAT);
        if (c[i] == '<' && c[i + 1] == '>')
            return new_token_type(TOK_LESSGREAT);
        if (c[i] == '>' && c[i + 1] == '|')
            return new_token_type(TOK_CLOBBER);
    }
    if (c[i] == '<')
        return new_token_type(TOK_LESS);
    if (c[i] == '>')
        return new_token_type(TOK_GREAT);
    return NULL;
}

struct token *lex_comments(char *c, size_t i)
{
    if (!c || !c[i] || c[i] != '#')
        return NULL;
    if (/*!c[i - 1]*/i == 0 || c[i - 1] == ' ' || c[i - 1] == '\n')
        return new_token_type(TOK_COMM);
    return NULL;
}

struct token *lex_uni_character(char *c, size_t i)
{
    if (!c || !c[i])
        return NULL;
    if (c[i] == ';' && c[i + 1] && c[i + 1] == ';')
        return new_token_type(KW_DSEMI);
    if (c[i] == ';')
        return new_token_type(TOK_SEMI);
    if (c[i] == '\n')
        return new_token_type(TOK_NEWLINE);
    // if (c[i] == '{')
    //     return new_token_type(TOK_LCURL);
    // if (c[i] == '}')
    //     return new_token_type(TOK_RCURL);
    if (c[i] == '(')
        return new_token_type(TOK_LPAREN);
    if (c[i] == ')')
        return new_token_type(TOK_RPAREN);
    if (c[i] == '&')
        return new_token_type(TOK_SEPAND);
    return NULL;
}

struct token *lex_assignment_value(char *c, size_t *i)
{
    size_t next_i = *i + 1;
    if (!c || !c[next_i])
        return NULL;
    char *value = substr(c, next_i, get_next_separator_index(c, next_i) - next_i);
    *i += strlen(value);
    struct token *new = new_token_type(TOK_WORD);
    new->value = value;
    return new;
}

enum token_type evaluate_keyword(char *c)
{
    if (is(c, "if"))
        return KW_IF;
    if (is(c, "then"))
        return KW_THEN;
    if (is(c, "elif"))
        return KW_ELIF;
    if (is(c, "else"))
        return KW_ELSE;
    if (is(c, "fi"))
        return KW_FI;
    if (is(c, "do"))
        return KW_DO;
    if (is(c, "done"))
        return KW_DONE;
    if (is(c, "for"))
        return KW_FOR;
    if (is(c, "while"))
        return KW_WHILE;
    if (is(c, "until"))
        return KW_UNTIL;
    if (is(c, "case"))
        return KW_CASE;
    if (is(c, "esac"))
        return KW_ESAC;
    if (is(c, "in"))
        return KW_IN;
    if (is(c, ";;"))
        return KW_DSEMI;
    return KW_UNKNOWN;
}

enum token_type evaluate_token(char *c)
{
    if (evaluate_keyword(c) != KW_UNKNOWN)
        return evaluate_keyword(c);
    if (is(c, "\n"))
        return TOK_NEWLINE;
    else if (is(c, "&&"))
        return TOK_AND;
    else if (is(c, "&"))
        return TOK_SEPAND;
    else if (is(c, "||"))
        return TOK_OR;
    else if (is(c, "|"))
        return TOK_PIPE;
    else if (is(c, ";"))
        return TOK_SEMI;
    else if (is(c, "("))
        return TOK_LPAREN;
    else if (is(c, ")"))
        return TOK_RPAREN;
    else if (is(c, "{"))
        return TOK_LCURL;
    else if (is(c, "}"))
        return TOK_RCURL;
    else if (is(c, "<<-"))
        return TOK_DLESSDASH;
    else if (is(c, "<<"))
        return TOK_DLESS;
    else if (is(c, "<>"))
        return TOK_LESSGREAT;
    else if (is(c, "<&"))
        return TOK_LESSAND;
    else if (is(c, "<"))
        return TOK_LESS;
    else if (is(c, ">>"))
        return TOK_DGREAT;
    else if (is(c, ">&"))
        return TOK_GREATAND;
    else if (is(c, ">|"))
        return TOK_CLOBBER;
    else if (is(c, ">"))
        return TOK_GREAT;
    else if (is(c, "!"))
        return TOK_NOT;
    else if (is(c, "#"))
        return TOK_COMM;
    return TOK_WORD;
}
