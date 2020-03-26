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
