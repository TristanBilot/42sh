#include "lexer/lexer.h"
#include "lexer/token.h"

#include <err.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }


    struct lexer *lexer = lexer_alloc(argv[1]);

    // printf("%s\n", token_str(lexer_peek(lexer)));
    // for (int i = 0; i < lexer->length; i++)
    //     printf("%s\n", token_str(lexer_pop(lexer)));


    struct token *token = NULL;
    while ((token = lexer_pop(lexer)))
    {
        printf("%s\n", token_str(token));
        if (token->type == TOKEN_EOF)
            break;

        token_free(token);
        token = NULL;
    }

    token_free(token);
    lexer_free(lexer);
}
