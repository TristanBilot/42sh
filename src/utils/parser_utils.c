#include "../lexer/token.h"
#include "parser_utils.h"

bool is_redirection(struct token *token)
{
    return (is_type(token, TOK_DLESSDASH) ||
        is_type(token, TOK_DLESS) ||
        is_type(token, TOK_LESSGREAT) ||
        is_type(token, TOK_LESSAND) ||
        is_type(token, TOK_LESS) ||
        is_type(token, TOK_DGREAT) ||
        is_type(token, TOK_GREATAND) ||
        is_type(token, TOK_CLOBBER) ||
        is_type(token, TOK_GREAT));
}
