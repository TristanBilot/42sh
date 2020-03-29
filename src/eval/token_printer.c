#include <stdio.h>
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/string.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }
    struct lexer *lexer = new_lexer(argv[1]);
    struct token *token = NULL;

    while ((token = pop(lexer)))
        printf("%s %s\n", type_to_str(token->type), token->value);

    free_lexer(lexer);
}
