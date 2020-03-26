#include <criterion/criterion.h>
#include <lexer/lexer.h>

Test(lexer, core_methods)
{
    struct lexer *lex = new_lexer("&& || ;;");
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(pop(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_OR);
    cr_assert(pop(lex)->type == TOK_OR);
    cr_assert(pop(lex)->type == TOK_DSEMI);
}

Test(lexer, input)
{
    const char *input = "find -name 'keta' && ls &||";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_AND);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_WORD);
}
