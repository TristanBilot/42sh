#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "utils/string.h"

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

Test(lexer, backslash)
{
    cr_assert(is(substr("azerty", 1, 6), "zerty"));

    // const char *input = "echo \n '\n' \\n";
    // struct lexer *lexer = new_lexer(input);
    // cr_assert(pop(lexer)->type == TOK_WORD);

    // struct token *t1 = pop(lexer);
    // cr_assert(t1->type == TOK_WORD);
    // cr_assert(is(t1->value, "n"));

    // struct token *t2 = pop(lexer);
    // cr_assert(t2->type == TOK_WORD);
    // cr_assert(is(t2->value, "\n"));
    
    // struct token *t3 = pop(lexer);
    // cr_assert(t3->type == TOK_WORD);
    // cr_assert(is(t3->value, "\n"));

    // const char *input2 = "\\\\a";
    // struct lexer *lexer2 = new_lexer(input2);
    // struct token *t4 = pop(lexer);
    // cr_assert(t4->type == TOK_WORD);
    // cr_assert(t4->value == "\a");

    // const char *input3 = "foo\\bar\\baz";
    // struct lexer *lexer3 = new_lexer(input2);
    // struct token *t5 = pop(lexer);
    // cr_assert(t5->type == TOK_WORD);
    // cr_assert(t5->value == "foaaz");
}

Test(lexer, io_number)
{
    const char *input = "echo 'hello' 2>&1";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(peek(lexer)->type == TOK_IONUMBER);
    cr_assert(is(pop(lexer)->value, "2"));

    cr_assert(pop(lexer)->type == TOK_GREATAND);
    cr_assert(peek(lexer)->type == TOK_IONUMBER);
    cr_assert(is(peek(lexer)->value, "1"));

    const char *input2 = "a 22>&2";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(peek(lexer2)->type == TOK_WORD);

    cr_assert(pop(lexer2)->type == TOK_GREATAND);
    cr_assert(peek(lexer2)->type == TOK_IONUMBER);
    cr_assert(is(peek(lexer2)->value, "2"));

}
