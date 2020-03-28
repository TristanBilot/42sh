#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/string.h"
#include "lexer/lexer.h"

char *lex_backslash(char *c, int i)
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

struct token *lex_great_less_and(const char *c, int i)
{
    if (!c || !c[i] || !c[i + 1])
        return NULL;
    if (c[i] == '<' && c[i + 1] == '&') /* <& */
        return new_token_type(TOK_LESSAND);
    if (c[i] == '>' && c[i + 1] == '&') /* >& */
        return new_token_type(TOK_GREATAND);
    return NULL;
}

struct token *lex_io_number(char *c, int i)
{
    if (!c || !c[i])
        return NULL;
    if (is_number(c[i]) && c[i + 1] == '>' &&
        ((c[i - 1] && (c[i - 1] == ' ')) || !c[i - 1])) /* 2> */
        return new_token_io_number(c[i]);
    if (c[i - 1] && c[i - 2] && c[i - 1] == '&'  && c[i - 2] == '>') /* >&2 */
    {
        if (is_number(c[i]))
        {
            if (c[i] > '2' || (c[i + 1] && is_number(c[i + 1]))) /* >&4 error */
                return new_token_error("bad file descriptor");
            else
                return new_token_io_number(c[i]);
        }
    }
    return NULL;
}

enum token_type evaluate_token(char *c)
{
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
    else if (is(c, ";;"))
        return TOK_DSEMI;
    else if (is(c, ";"))
        return TOK_SEMI;
    else if (is(c, "("))
        return TOK_LPAREN;
    else if (is(c, ")"))
        return TOK_RPAREN;
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
    return TOK_WORD;
}
