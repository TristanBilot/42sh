#include <stdio.h>
#include <errno.h>
#include "utils/string.h"

char *type_to_str(int type)
{
    char *types[] = {
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
    char *dest = NULL;
    if (len > 0)
    {
        dest = calloc(len + 10, 1);
        if (dest != NULL)
            strncat(dest, src + pos, len);
    }
    return dest;
}

void error(char *msg)
{
    printf("%s\n", msg);
}
