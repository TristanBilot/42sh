#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "parser/parser.h"
#include "utils/string.h"


Test(parser, parse_redirection)
{
    struct parser *parser = init_parser(new_lexer("1>2"));
    void *ast = NULL;
    cr_assert(!parse_redirection(parser, ast));

    struct parser *parser11 = init_parser(new_lexer("1<2"));
    void *ast11 = NULL;
    cr_assert(!parse_redirection(parser11, ast11));

    /*struct parser *parser2 = init_parser(new_lexer("1<>2"));                  //on voit ça après
    void *ast2 = NULL;
    cr_assert(!parse_redirection(parser2, ast2));*/

/*
    struct parser *parser3 = init_parser(new_lexer("1>&2"));
    void *ast3 = NULL;
    cr_assert(!parse_redirection(parser3, ast3));

    struct parser *parser4 = init_parser(new_lexer("1 > &2"));
    void *ast4 = NULL;
    cr_assert(parse_redirection(parser4, ast4));

    struct parser *parser5= init_parser(new_lexer("1>>2"));
    void *ast5 = NULL;
    cr_assert(!parse_redirection(parser5, ast5));

    struct parser *parser6= init_parser(new_lexer("1<<2"));
    void *ast6 = NULL;
    cr_assert(!parse_redirection(parser6, ast6));

    struct parser *parser7= init_parser(new_lexer("1>>2"));
    void *ast7 = NULL;
    cr_assert(!parse_redirection(parser7, ast7));

    struct parser *parser8= init_parser(new_lexer("1<<-2"));
    void *ast8 = NULL;
    cr_assert(!parse_redirection(parser8, ast8));

    struct parser *parser9= init_parser(new_lexer("1>|2"));
    void *ast9 = NULL;
    cr_assert(!parse_redirection(parser9, ast9));

    struct parser *parser10 = init_parser(new_lexer("1<&2"));
    void *ast10 = NULL;
    cr_assert(!parse_redirection(parser10, ast10));
*/
}

Test(parser, parse_simple_command){
    //one simple command
    struct parser *parser = init_parser(new_lexer("ls"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));

    // simple command with param
    struct parser *parser2 = init_parser(new_lexer("echo test"));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));

    // simple command with assigment word
    /*struct parser *parser3 = init_parser(new_lexer("a=1 echo a"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));*/

    
}

Test(parser, parser_simple_command2){

    struct parser *parser = init_parser(new_lexer("\n"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));
 
    struct parser *parser2 = init_parser(new_lexer(" "));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));

    struct parser *parser3 = init_parser(new_lexer("ls;echo toto"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));

    struct parser *parser4 = init_parser(new_lexer(""));
    void *ast4 = NULL;
    cr_assert(!parse_input(parser4, ast4));

    struct parser *parser5 = init_parser(new_lexer("ls;echo toto;ls"));
    void *ast5 = NULL;
    cr_assert(!parse_input(parser5, ast5));

    struct parser *parser6 = init_parser(new_lexer("ls;  echo toto;ls"));
    void *ast6 = NULL;
    cr_assert(!parse_input(parser6, ast6));

    struct parser *parser7 = init_parser(new_lexer("ls;\n\necho toto;ls\n"));
    void *ast7 = NULL;
    cr_assert(!parse_input(parser7, ast7));

    struct parser *parser8 = init_parser(new_lexer("ls \n echo toto \n ls\n"));
    void *ast8 = NULL;
    cr_assert(!parse_input(parser8, ast8));

}
/*
Test(parser, parse_simple_if){
    // simple if
    struct parser *parser = init_parser(new_lexer("if a then b fi"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));
    
    // if else
    struct parser *parser2 = init_parser(new_lexer("if a then b else c fi"));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));
    
    // if elif else
    struct parser *parser3 = init_parser(new_lexer("if a then b elif c else d fi"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));
    
}
*/

/*Test(parser, parser_and_or_simple){
    struct parser *parser = init_parser(new_lexer("ls | cat test"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));

    struct parser *parser2 = init_parser(init_parser(new_lexer("ls || cat test")));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));

    struct parser *parser3 = init_parser(new_lexer("ls & cat Makefile"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));

    struct parser *parser4 = init_parser(new_lexer("ls && cat Makefile"));
    void *ast4 = NULL;
    cr_assert(!parse_input(parser4, ast4));
    
}
*/

/*
Test(parser, parser_multi_logical){

    struct parser *parser = init_parser(new_lexer("ls | echo test || cat test"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));

    struct parser *parser2= init_parser(new_lexer("ls || echo test || cat test"));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));

    struct parser *parser3 = init_parser(new_lexer("ls && echo test && | cat test"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));

}
Test(parser, parser_){

     struct parser *parser = init_parser(new_lexer("/n"));
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));
 
    struct parser *parser2 = init_parser(new_lexer(" "));
    void *ast2 = NULL;
    cr_assert(!parse_input(parser2, ast2));

    struct parser *parser3 = init_parser(new_lexer("ls;echo toto"));
    void *ast3 = NULL;
    cr_assert(!parse_input(parser3, ast3));
}
*/
