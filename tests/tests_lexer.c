#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "utils/string_utils.h"
#include "../garbage_collector/garbage_collector.h"

Test(lexer, basic_tokens)
{
    new_garbage_collector();
    char input[] = "&& || ;;";
    struct lexer *lex = new_lexer(input);
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_AND);
    cr_assert(pop(lex)->type == TOK_AND);
    cr_assert(peek(lex)->type == TOK_OR); 
    cr_assert(pop(lex)->type == TOK_OR);
    cr_assert(pop(lex)->type == KW_DSEMI);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, basic_word_tokens)
{
    /* La flemme de le faire, il faut gérer le ;|| comme un word */

    // new_garbage_collector();
    // char input[] = "find -name 'keta' && ls ;||";
    // struct lexer *lexer = new_lexer(input);
    // cr_assert(pop(lexer)->type == TOK_WORD);
    // cr_assert(pop(lexer)->type == TOK_WORD);
    // cr_assert(pop(lexer)->type == TOK_WORD);
    // cr_assert(pop(lexer)->type == TOK_AND);
    // cr_assert(pop(lexer)->type == TOK_WORD);
    // cr_assert(pop(lexer)->type == TOK_WORD);
    // free_garbage_collector();
    // free(garbage_collector);
}

Test(lexer, newline)
{
    new_garbage_collector();
    char input[] = "word \n another";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_NEWLINE);
    cr_assert(pop(lexer)->type == TOK_WORD);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, eof)
{
    new_garbage_collector();
    char input[] = "word";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_EOF);
    cr_assert(pop(lexer) == NULL);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, backslash)
{
    new_garbage_collector();
    // char input[] = "echo \n '\n' \\n";
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

    // char input2[] = "\\\\a";
    // struct lexer *lexer2 = new_lexer(input2);
    // struct token *t4 = pop(lexer);
    // cr_assert(t4->type == TOK_WORD);
    // cr_assert(t4->value == "\a");

    // char input3[] = "foo\\bar\\baz";
    // struct lexer *lexer3 = new_lexer(input2);
    // struct token *t5 = pop(lexer);
    // cr_assert(t5->type == TOK_WORD);
    // cr_assert(t5->value == "foaaz");
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, io_number)
{
    new_garbage_collector();
    char input[] = "echo 'hello' 2>&1";
    struct lexer *lexer = new_lexer(input);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(pop(lexer)->type == TOK_WORD);
    cr_assert(peek(lexer)->type == TOK_IONUMBER);
    cr_assert(is(pop(lexer)->value, "2"));

    cr_assert(pop(lexer)->type == TOK_GREATAND);
    cr_assert(peek(lexer)->type == TOK_WORD);
    cr_assert(is(peek(lexer)->value, "1"));
    free_garbage_collector();

    char input2[] = "a 22>&2";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);

    cr_assert(pop(lexer2)->type == TOK_GREATAND);
    cr_assert(peek(lexer2)->type == TOK_WORD);
    cr_assert(is(peek(lexer2)->value, "2"));
    free_garbage_collector();

    char input3[] = "aa 2>&3";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(is(pop(lexer3)->value, "2"));

    cr_assert(pop(lexer3)->type == TOK_GREATAND);
    cr_assert(peek(lexer3)->type == TOK_WORD);
    free_garbage_collector();

    char input4[] = "a 2>&22";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    // cr_assert(is(pop(lexer3)->value, "2"));
    pop(lexer4);
    cr_assert(pop(lexer4)->type == TOK_GREATAND);
    cr_assert(peek(lexer4)->type == TOK_WORD);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, spaced_redirections)
{
    new_garbage_collector();
    char input1[] = "echo 'hello' > file";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_GREAT);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();

    char input2[] = "echo 'hello' >> file";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_DGREAT);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    free_garbage_collector();

    char input3[] = "echo 'hello' << file";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_DLESS);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    free_garbage_collector();

    char input4[] = "echo 'hello' < file";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LESS);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    free_garbage_collector();

    char input5[] = "echo 'hello' <<- file";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_DLESSDASH);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    free_garbage_collector();

    char input6[] = "echo 'hello' >| file";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_CLOBBER);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    free_garbage_collector();

    char input7[] = "echo 'hello' <> file";
    struct lexer *lexer7 = new_lexer(input7);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_LESSGREAT);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    free_garbage_collector();

    char input8[] = "echo 'hello' 2>> file";
    struct lexer *lexer8 = new_lexer(input8);
    cr_assert(pop(lexer8)->type == TOK_WORD);
    cr_assert(pop(lexer8)->type == TOK_WORD);
    cr_assert(pop(lexer8)->type == TOK_IONUMBER);
    cr_assert(pop(lexer8)->type == TOK_DGREAT);
    cr_assert(pop(lexer8)->type == TOK_WORD);
    free_garbage_collector();

    char input9[] = "echo 'hello' 2<< file";
    struct lexer *lexer9 = new_lexer(input9);
    cr_assert(pop(lexer9)->type == TOK_WORD);
    cr_assert(pop(lexer9)->type == TOK_WORD);
    cr_assert(pop(lexer9)->type == TOK_IONUMBER);
    cr_assert(pop(lexer9)->type == TOK_DLESS);
    cr_assert(pop(lexer9)->type == TOK_WORD);
    free_garbage_collector();

    char input10[] = "echo 'hello' 2>file";
    struct lexer *lexer10 = new_lexer(input10);
    cr_assert(pop(lexer10)->type == TOK_WORD);
    cr_assert(pop(lexer10)->type == TOK_WORD);
    cr_assert(pop(lexer10)->type == TOK_IONUMBER);
    cr_assert(pop(lexer10)->type == TOK_GREAT);
    cr_assert(pop(lexer10)->type == TOK_WORD);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, no_spaced_redirections)
{
    new_garbage_collector();
    char input1[] = "echo 'hello'>file";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_GREAT);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();

    char input2[] = "echo 'hello'>>file";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_DGREAT);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    free_garbage_collector();

    char input3[] = "echo 'hello'<<file";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_DLESS);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    free_garbage_collector();

    char input4[] = "echo 'hello'<file";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LESS);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    free_garbage_collector();

    char input5[] = "echo 'hello'<<-file";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_DLESSDASH);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    free_garbage_collector();

    char input6[] = "echo 2>|file";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    cr_assert(pop(lexer6)->type == TOK_IONUMBER);
    cr_assert(pop(lexer6)->type == TOK_CLOBBER);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    free_garbage_collector();

    char input7[] = "echo 23<>file";
    struct lexer *lexer7 = new_lexer(input7);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    cr_assert(pop(lexer7)->type == TOK_LESSGREAT);
    cr_assert(pop(lexer7)->type == TOK_WORD);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, semicolon)
{
    new_garbage_collector();
    char input1[] = ";;";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_DSEMI);
    free_garbage_collector();

    char input2[] = ";";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_SEMI);
    free_garbage_collector();

    char input3[] = "case answer in 1;;";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == KW_CASE);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == KW_IN);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == KW_DSEMI);
    free_garbage_collector();

    char input4[] = "case answ;er ;;in 1;;";
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
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, not)
{
    new_garbage_collector();
    char input1[] = "if ! true";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_NOT);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, curly_braces)
{
    new_garbage_collector();
    char input1[] = "if { a -eq b }";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_LCURL);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_RCURL);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, assignment_word)
{
    new_garbage_collector();
    char input1[] = "var=123";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(peek(lexer1)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer1)->value, "var"));
    cr_assert(is(peek(lexer1)->value, "123"));
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();

    char input2[] = "=223";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(peek(lexer2)->type == TOK_WORD);
    free_garbage_collector();

    char input3[] = "a=a=42";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(peek(lexer3)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer3)->value, "a"));
    cr_assert(peek(lexer3)->type == TOK_WORD);
    cr_assert(is(pop(lexer3)->value, "a=42"));
    free_garbage_collector();

    char input4[] = "var1=hello;var2=123\nvar3=$1";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(peek(lexer4)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer4)->value, "var1"));
    cr_assert(peek(lexer4)->type == TOK_WORD);
    cr_assert(is(pop(lexer4)->value, "hello"));
    cr_assert(pop(lexer4)->type == TOK_SEMI);
    cr_assert(peek(lexer4)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer4)->value, "var2"));
    cr_assert(peek(lexer4)->type == TOK_WORD);
    cr_assert(is(pop(lexer4)->value, "123"));
    cr_assert(pop(lexer4)->type == TOK_NEWLINE);
    cr_assert(peek(lexer4)->type == TOK_ASS_WORD);
    cr_assert(is(pop(lexer4)->value, "var3"));
    cr_assert(peek(lexer4)->type == TOK_WORD);
    cr_assert(is(pop(lexer4)->value, "$1"));
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, parenthesis)
{
    new_garbage_collector();
    char input1[] = "(some content)";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_LPAREN);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_RPAREN);
    free_garbage_collector();

    char input2[] = "(if elif)";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_LPAREN);
    cr_assert(pop(lexer2)->type == KW_IF);
    cr_assert(pop(lexer2)->type == KW_ELIF);
    cr_assert(pop(lexer2)->type == TOK_RPAREN);
    free_garbage_collector();

    char input3[] = "((if elif)))";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_LPAREN);
    cr_assert(pop(lexer3)->type == TOK_LPAREN);
    cr_assert(pop(lexer3)->type == KW_IF);
    cr_assert(pop(lexer3)->type == KW_ELIF);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);
    cr_assert(pop(lexer3)->type == TOK_RPAREN);
    free_garbage_collector();

    char input4[] = "word((if)word";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_LPAREN);
    cr_assert(pop(lexer4)->type == TOK_LPAREN);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_RPAREN);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    free_garbage_collector();
    
    char input5[] = "if(";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_LPAREN);
    free_garbage_collector();

    char input6[] = ")if";
    struct lexer *lexer6 = new_lexer(input6);
    cr_assert(pop(lexer6)->type == TOK_RPAREN);
    cr_assert(pop(lexer6)->type == TOK_WORD);
    free_garbage_collector();

    char input7[] = "(if case b then(echo toto))";
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
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, comments)
{
    new_garbage_collector();
    char input1[] = "#some comments";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_COMM);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();

    char input2[] = "a # =b";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_COMM);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    free_garbage_collector();

    char input3[] = "a#b";
    struct lexer *lexer3 = new_lexer(input3);
    cr_assert(pop(lexer3)->type == TOK_WORD);
    cr_assert(pop(lexer3)->type == TOK_EOF);
    free_garbage_collector();

    char input4[] = "ab #";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_WORD);
    cr_assert(pop(lexer4)->type == TOK_COMM);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, if_test)
{
    new_garbage_collector();
    char input1[] = "if\na";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_NEWLINE);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    free_garbage_collector();

    char input2[] = "if\na\nthen\nb";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == KW_IF);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == KW_THEN);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    free_garbage_collector();

    char input3[] = "\nif\na\n\n\nthen\nb";
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
    free_garbage_collector();

    char input4[] = "\n";
    struct lexer *lexer4 = new_lexer(input4);
    cr_assert(pop(lexer4)->type == TOK_NEWLINE);
    cr_assert(pop(lexer4)->type == TOK_EOF);
    free_garbage_collector();

    char input5[] = "\nifs sif\n";
    struct lexer *lexer5 = new_lexer(input5);
    cr_assert(pop(lexer5)->type == TOK_NEWLINE);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_WORD);
    cr_assert(pop(lexer5)->type == TOK_NEWLINE);
    cr_assert(pop(lexer5)->type == TOK_EOF);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, dollar)
{
    new_garbage_collector();
    char input1[] = "echo $1";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_WORD);
    cr_assert(pop(lexer1)->type == TOK_EOF);
    free_garbage_collector();
    free(garbage_collector);
}

Test(lexer, hard_stuck)
{
    new_garbage_collector();
    char input1[] = "if;done;fi;";
    struct lexer *lexer1 = new_lexer(input1);
    cr_assert(pop(lexer1)->type == KW_IF);
    cr_assert(pop(lexer1)->type == TOK_SEMI);
    cr_assert(pop(lexer1)->type == KW_DONE);
    cr_assert(pop(lexer1)->type == TOK_SEMI);
    cr_assert(pop(lexer1)->type == KW_FI);
    cr_assert(pop(lexer1)->type == TOK_SEMI);
    free_garbage_collector();

    char input2[] = "\nifs;\n && done&;fi\n&";
    struct lexer *lexer2 = new_lexer(input2);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_WORD);
    cr_assert(pop(lexer2)->type == TOK_SEMI);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_AND);
    cr_assert(pop(lexer2)->type == KW_DONE);
    cr_assert(pop(lexer2)->type == TOK_SEPAND);
    cr_assert(pop(lexer2)->type == TOK_SEMI);
    cr_assert(pop(lexer2)->type == KW_FI);
    cr_assert(pop(lexer2)->type == TOK_NEWLINE);
    cr_assert(pop(lexer2)->type == TOK_SEPAND);
    free_garbage_collector();

    free(garbage_collector);
}
