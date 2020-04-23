#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "utils/string_utils.h"

Test(lexer, basic_tokens)
{
    struct lexer *lex = new_lexer("&& || ;;");
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(pop(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_OR); 
    cr_assert(pop(lex)->type == TOK_OR);
    cr_assert(pop(lex)->type == KW_DSEMI);
}

Test(lexer, basic_word_tokens)
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

Test(lexer, newline)
{
    const char *input = "word \n another";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_NEWLINE);
    cr_assert(pop(lexer)->type == TOK_WORD);
}

Test(lexer, eof)
{
    const char *input = "word";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_EOF);
    cr_assert(pop(lexer) == NULL);
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
    cr_assert(peek(lexer)->type == TOK_WORD);
    cr_assert(is(peek(lexer)->value, "1"));

    const char *input2 = "a 22>&2";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    cr_assert(pop(lexer2)->type == TOK_GREATAND);
    cr_assert(peek(lexer2)->type == TOK_WORD);
    cr_assert(is(peek(lexer2)->value, "2"));

    const char *input3 = "aa 2>&3";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(is(pop(lexer3)->value, "2"));

    cr_assert(pop(lexer3)->type == TOK_GREATAND);
    cr_assert(peek(lexer3)->type == TOK_WORD);

    const char *input4 = "a 2>&22";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    // cr_assert(is(pop(lexer3)->value, "2"));
    pop(lexer4);
    cr_assert(pop(lexer4)->type == TOK_GREATAND);
    cr_assert(peek(lexer4)->type == TOK_WORD);
}

Test(lexer, spaced_redirections)
{
    const char *input1 = "echo 'hello' > file";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_GREAT);
    cr_assert(pop(lexer1)->type == TOK_WORD);

    const char *input2 = "echo 'hello' >> file";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_DGREAT);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    const char *input3 = "echo 'hello' << file";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_DLESS);
    cr_assert(pop(lexer3)->type == TOK_WORD);

    const char *input4 = "echo 'hello' < file";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LESS);
    cr_assert(pop(lexer4)->type == TOK_WORD);

    const char *input5 = "echo 'hello' <<- file";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_DLESSDASH);
    cr_assert(pop(lexer5)->type == TOK_WORD);

    const char *input6 = "echo 'hello' >| file";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_CLOBBER);
    cr_assert(pop(lexer6)->type == TOK_WORD);

    const char *input7 = "echo 'hello' <> file";
    struct lexer *lexer7 = new_lexer(input7);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_LESSGREAT);
    cr_assert(pop(lexer7)->type == TOK_WORD);
}

Test(lexer, no_spaced_redirections)
{
    const char *input1 = "echo 'hello'>file";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_GREAT);
    cr_assert(pop(lexer1)->type == TOK_WORD);

    const char *input2 = "echo 'hello'>>file";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_DGREAT);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    const char *input3 = "echo 'hello'<<file";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_DLESS);
    cr_assert(pop(lexer3)->type == TOK_WORD);

    const char *input4 = "echo 'hello'<file";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LESS);
    cr_assert(pop(lexer4)->type == TOK_WORD);

    const char *input5 = "echo 'hello'<<-file";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_DLESSDASH);
    cr_assert(pop(lexer5)->type == TOK_WORD);

    const char *input6 = "echo 2>|file";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_IONUMBER);
    cr_assert(pop(lexer6)->type == TOK_CLOBBER);
    cr_assert(pop(lexer6)->type == TOK_WORD);

    const char *input7 = "echo 23<>file";
    struct lexer *lexer7 = new_lexer(input7);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_LESSGREAT);
    cr_assert(pop(lexer7)->type == TOK_WORD);
}

Test(lexer, semicolon)
{
    const char *input1 = ";;";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_DSEMI);

    const char *input2 = ";";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_SEMI);

    const char *input3 = "case answer in 1;;";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == KW_CASE);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == KW_IN);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == KW_DSEMI);

    const char *input4 = "case answ;er ;;in 1;;";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == KW_CASE);
    cr_assert(peek(lexer4)->type == TOK_WORD);
    cr_assert(is(pop(lexer4)->value, "answ"));
    cr_assert(pop(lexer4)->type == TOK_SEMI);
    cr_assert(peek(lexer4)->type == TOK_WORD);
    cr_assert(is(pop(lexer4)->value, "er"));
    cr_assert(pop(lexer4)->type == KW_DSEMI);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == KW_DSEMI);
}

Test(lexer, not)
{
    const char *input1 = "if ! true";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_NOT);
    cr_assert(pop(lexer1)->type == TOK_WORD);
}

Test(lexer, curly_braces)
{
    const char *input1 = "if { a -eq b }";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_LCURL);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_RCURL);
}

Test(lexer, assignment_word)
{
    const char *input1 = "var=123";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(peek(lexer1)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer1)->value, "var"));
    cr_assert(is(peek(lexer1)->value, "123"));
    cr_assert(pop(lexer1)->type == TOK_WORD);

    const char *input2 = "=223";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(peek(lexer2)->type == TOK_WORD);

    const char *input3 = "a=a=42";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(peek(lexer3)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer3)->value, "a"));
    cr_assert(peek(lexer3)->type == TOK_WORD);
    cr_assert(is(pop(lexer3)->value, "a=42"));
}

Test(lexer, parenthesis)
{
    const char *input1 = "(some content)";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_LPAREN);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_RPAREN);

    const char *input2 = "(if elif)";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_LPAREN);
    cr_assert(pop(lexer2)->type == KW_IF);
    cr_assert(pop(lexer2)->type == KW_ELIF);
    cr_assert(pop(lexer2)->type == TOK_RPAREN);

    const char *input3 = "((if elif)))";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_LPAREN);
    cr_assert(pop(lexer3)->type == TOK_LPAREN);
    cr_assert(pop(lexer3)->type == KW_IF);
    cr_assert(pop(lexer3)->type == KW_ELIF);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);

    const char *input4 = "word((if)word";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LPAREN);
    cr_assert(pop(lexer4)->type == TOK_LPAREN);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_RPAREN);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    
    const char *input5 = "if(";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_LPAREN);

    const char *input6 = ")if";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_RPAREN);
    cr_assert(pop(lexer6)->type == TOK_WORD);

    const char *input7 = "(if case b then(echo toto))";
    struct lexer *lexer7 = new_lexer(input7);
    cr_assert(pop(lexer7)->type == TOK_LPAREN);
    cr_assert(pop(lexer7)->type == KW_IF);
    cr_assert(pop(lexer7)->type == KW_CASE);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_LPAREN);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_RPAREN);
    cr_assert(pop(lexer7)->type == TOK_RPAREN);
}

Test(lexer, comments)
{
    const char *input1 = "#some comments";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_COMM);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);

    const char *input2 = "a # =b";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_COMM);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    const char *input3 = "a#b";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_EOF);

    const char *input4 = "ab #";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_COMM);
}

Test(lexer, stuck_newlines)
{
    const char *input1 = "if\na";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_NEWLINE);
    cr_assert(pop(lexer1)->type == TOK_WORD);

    const char *input2 = "if\na\nthen\nb";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == KW_IF);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == KW_THEN);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    const char *input3 = "\nif\na\n\n\nthen\nb";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == KW_IF);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == KW_THEN);
    cr_assert(pop(lexer3)->type == TOK_NEWLINE);
    cr_assert(pop(lexer3)->type == TOK_WORD);

    const char *input4 = "\n";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_NEWLINE);
    cr_assert(pop(lexer4)->type == TOK_EOF);

    const char *input5 = "\nifs sif\n";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_NEWLINE);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_NEWLINE);
    cr_assert(pop(lexer5)->type == TOK_EOF);
}
