#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"

char *type_to_str(int type)
{
    char *types[] =
    {
        "TOK_ERROR",
        "TOK_NEWLINE",
        "TOK_EOF",
        "TOK_AND",
        "TOK_SEPAND",
        "TOK_OR",
        "TOK_PIPE",
        "TOK_SEMI",
        "TOK_LPAREN",
        "TOK_RPAREN",
        "TOK_LCURL",
        "TOK_RCURL",
        "TOK_DLESSDASH",
        "TOK_DLESS",
        "TOK_LESSGREAT",
        "TOK_LESSAND",
        "TOK_LESS",
        "TOK_DGREAT",
        "TOK_GREATAND",
        "TOK_CLOBBER",
        "TOK_ASS_WORD",
        "TOK_GREAT",
        "TOK_IONUMBER",
        "TOK_NOT",
        "TOK_COMM",
        "TOK_WORD",
        "KW_IF",
        "KW_THEN",
        "KW_ELSE",
        "KW_ELIF",
        "KW_FI",
        "KW_DO",
        "KW_DONE",
        "KW_FOR",
        "KW_WHILE",
        "KW_UNTIL",
        "KW_CASE",
        "KW_ESAC",
        "KW_IN",
        "KW_DSEMI",
        "KW_UNKNOWN"
    };
    return types[type];
}

int is(const char *a, const char *b)
{
    return a && b && strcmp(a, b) == 0;
}

int is_number(char c)
{
    return c >= '0' && c <= '9';
}

char *substr(char *src, int pos, int len)
{
    if (!src)
        return NULL;
    char *dest = NULL;
    if (len > 0)
    {
        dest = xcalloc(len + 10, 1);
        if (dest != NULL)
            strncat(dest, src + pos, len);
    }
    return dest;
}

char *my_strdup(const char *c)
{
    char *dup = xmalloc(strlen(c) + 1);
    if (dup != NULL)
        strcpy(dup, c);
    return dup;
}

void error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}


bool expr_is_number(char *expr)
{
    for (size_t i = 0; i < strlen(expr); i++)
        if (!is_number(expr[i]))
            return false;
    return true;
}
