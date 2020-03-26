#include <criterion/criterion.h>
#include <token.h>

Test(lexer, test) {
    const char *input = "";
    struct lexer *lexer = new_lexer(input);
    cr_assert(lexer_pop(lexer) == TOK_EOF);
}