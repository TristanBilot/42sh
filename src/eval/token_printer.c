#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/string.h"

#include <err.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }
    printf("%s\n", "aa\n");
    struct lexer *lexer = new_lexer(argv[1]);

    // printf("%s\n", token_str(lexer_peek(lexer)));
    // for (int i = 0; i < lexer->length; i++)
    //     printf("%s\n", token_str(pop(lexer)));

    struct token *token = NULL;
    while ((token = pop(lexer)))
    {
        printf("%s %s\n", type_to_str(token->type), token->value);
        if (token->type == TOK_EOF)
            break;

    }
    free_lexer(lexer);
}
