#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "parser/parser.h"
#include "utils/string.h"

Test(parser, parse_simple_command){
    //one simple command
    struct lexer *lexer = new_lexer("ls");
    struct parser *parser = init_parser(lexer);
    void *ast = NULL;
    cr_assert(!parse_input(parser, ast));
    /*
    // simple command with param
    struct lexer *lexer = new_lexer("echo test");
    struct parser *parser = init_parser(lexer);
    struct node_input *ast = NULL;
    cr_assert(!parse_input(parser, ast));

    // simple command with assigment word
    struct lexer *lexer = new_lexer("a=1 echo a");
    struct parser *parser = init_parser(lexer);
    struct node_input *ast = NULL;
    cr_assert(!parse_input(parser, ast));
    */
}
/*
Test(parser, parse_simple_if){
    // simple if
    struct lexer *lexer = new_lexer("if a then b fi");
    struct parser *parser = init_parser(lexer);
    struct node_input *ast = NULL;
    cr_assert(!parse_input(parser, ast));
    
    // if else
    struct lexer *lexer = new_lexer("if a then b else c fi");
    struct parser *parser = init_parser(lexer);
    struct node_input *ast = NULL;
    cr_assert(!parse_input(parser, ast));
    
    // if elif else
    struct lexer *lexer = new_lexer("if a then b elif c else d fi");
    struct parser *parser = init_parser(lexer);
    struct node_input *ast = NULL;
    cr_assert(!parse_input(parser, ast));
}

Test(parser, parser_and_or_simple){
    struct lexer *lexer = new_lexer("ls | cat test");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));

    struct lexer *lexer = new_lexer("ls || cat test ");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));

    struct lexer *lexer = new_lexer("ls & cat Makefile");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));

    struct lexer *lexer = new_lexer("ls && cat Makefile");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
    
}

Test(parser, parser_){
    struct lexer *lexer = new_lexer("/n");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
Test(parser, parser_){
    struct lexer *lexer = new_lexer(" ");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
Test(parser, parser_){
    struct lexer *lexer = new_lexer("ls;echo toto");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
Test(parser, parser_){
    struct lexer *lexer = new_lexer("ls | echo test | cat test");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
Test(parser, parser_){
    struct lexer *lexer = new_lexer("ls || echo test || cat test");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}

Test(parser, parser_){
    struct lexer *lexer = new_lexer("ls & echo test & cat test");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
Test(parser, parser_){
    struct lexer *lexer = new_lexer("ls && echo test && cat test");
    struct parser *parser = init_parser(lexer);
    cr_assert(!parse_input(parser, ast));
}
*/
