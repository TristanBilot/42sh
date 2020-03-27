#include <string.h>
#include "utils/string.h"

char *type_to_str(int type)
{
    char *types[] = {
        "TOK_NONE",
        "TOK_NEWLINE",
        "TOK_EOF",	
        "TOK_AND",	
        "TOK_SEPAND",	
        "TOK_OR",		   
        "TOK_PIPE",	
        "TOK_DSEMI",	
        "TOK_SEMI",	
        "TOK_LPAREN",	
        "TOK_RPAREN",	
        "TOK_DLESSDASH",
        "TOK_DLESS",	
        "TOK_LESSGREAT",
        "TOK_LESSAND",
        "TOK_LESS",	
        "TOK_DGREAT",	
        "TOK_GREATAND",
        "TOK_CLOBBER",
        "TOK_GREAT",	
        "TOK_IONUMBER",
        "TOK_WORD"
    };
    return types[type];
}

int is(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
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
