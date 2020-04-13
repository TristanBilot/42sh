#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/string.h"

Test(parser, parse_redirection)
{
    struct parser *parser = init_parser(new_lexer("1>2"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    struct parser *parser11 = init_parser(new_lexer("1<2"));
    void *ast11 = NULL;
    bool result11 = parse_input(parser11, &ast11);
    // free_parser(parser11);
    cr_assert(!result11);
}

Test(parser, stuff) {
    struct parser *parser2 = init_parser(new_lexer("1<>2"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);

    struct parser *parser3 = init_parser(new_lexer("1>&2"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);
    //struct parser *parser4 = init_parser(new_lexer("1 > &2"));                  //on voit ça après
    //void *ast4 = NULL;
    //cr_assert(parse_input(parser4, &ast4)); 

    struct parser *parser5= init_parser(new_lexer("1>>2"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser5);
    cr_assert(!result5);


    struct parser *parser6= init_parser(new_lexer("1<<2"));
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser6);
    cr_assert(!result6);

    struct parser *parser7= init_parser(new_lexer("1>>2"));
    void *ast7 = NULL;
    bool result7 = parse_input(parser7, &ast7);
    // free_parser(parser7);
    cr_assert(!result7);

    struct parser *parser8= init_parser(new_lexer("1<<-2"));
    void *ast8 = NULL;
    bool result8 = parse_input(parser8, &ast8);
    // free_parser(parser8);
    cr_assert(!result8);

    struct parser *parser9= init_parser(new_lexer("1>|2"));
    void *ast9 = NULL;
    bool result9 = parse_input(parser9, &ast9);
    // free_parser(parser9);
    cr_assert(!result9);

    struct parser *parser10 = init_parser(new_lexer("1<&2"));
    void *ast10 = NULL;
    bool result10 = parse_input(parser10, &ast10);
    // free_parser(parser10);
    cr_assert(!result10);
}

Test(parser, parse_simple_command)
{
    /// one simple command
    struct parser *parser = init_parser(new_lexer("ls"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    // simple command with param
     struct parser *parser2 = init_parser(new_lexer("echo test"));
     void *ast2 = NULL;
     bool result2 = parse_input(parser2, &ast2);
     // free_parser(parser);
     cr_assert(!result2);
}

Test(parser, parser_assigment_word)
{
    // one assigment word
    struct parser *parser = init_parser(new_lexer("a=1"));
    void *ast = NULL; 
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);
    
    // simple command with assigment word
    struct parser *parser2 = init_parser(new_lexer("a=1 echo a"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);
}

Test(parser, parser_simple_command2){

    struct parser *parser = init_parser(new_lexer("\n"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);
 
    struct parser *parser2 = init_parser(new_lexer(" "));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);

    struct parser *parser3 = init_parser(new_lexer("ls;echo toto"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);


    struct parser *parser4 = init_parser(new_lexer(""));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser4);
    cr_assert(!result4);


    struct parser *parser5 = init_parser(new_lexer("ls;echo toto;ls"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser5);
    cr_assert(!result5);


    struct parser *parser6 = init_parser(new_lexer("ls;  echo toto;ls"));
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser6);
    cr_assert(!result6);


    struct parser *parser7 = init_parser(new_lexer("ls;\n\necho toto;ls\n"));
    void *ast7 = NULL;
    bool result7 = parse_input(parser7, &ast7);
    // free_parser(parser7);
    cr_assert(!result7);

    struct parser *parser8 = init_parser(new_lexer("ls \n echo toto \n ls\n"));
    void *ast8 = NULL;
    bool result8 = parse_input(parser8, &ast8);
    // free_parser(parser8);
    cr_assert(!result8);


}

Test(parser, parse_simple_if){
    // simple if
    struct parser *parser = init_parser(new_lexer("if a then b fi"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);


    // if else
    struct parser *parser2 = init_parser(new_lexer("if a then b else c fi"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);


    // if elif else
    struct parser *parser3 = init_parser(new_lexer("if a then b elif c then d else e fi"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);


    // if elif
    struct parser *parser4 = init_parser(new_lexer("if a then b elif c then d fi"));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser4);
    cr_assert(!result4);

    // if elif elif elif else
    struct parser *parser5 = init_parser(new_lexer("if a then b elif c then d elif e then f else g fi"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser5);
    cr_assert(!result5);

    struct parser *parser6 = init_parser(new_lexer("if a then b  c then d elif e then f else g fi"));
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser5);
    cr_assert(result6);

    struct parser *parser7 = init_parser(new_lexer("if a then b else c"));
    void *ast7 = NULL;
    bool result7 = parse_input(parser7, &ast7);
    // free_parser(parser5);
    cr_assert(result7);
}


Test(parser, parser_and_or_simple){
    
    struct parser *parser = init_parser(new_lexer("ls | cat test"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    

    struct parser *parser2 = init_parser(new_lexer("ls | cat test"));                                                // ne pas oublier de tester les ! (not)
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);


    struct parser *parser3 = init_parser(new_lexer("ls & cat Makefile"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);


    struct parser *parser4 = init_parser(new_lexer("ls && cat Makefile"));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser4);
    cr_assert(!result4);

    struct parser *parser5 = init_parser(new_lexer("ls && || cat Makefile"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser5);
    cr_assert(result5);

    struct parser *parser6 = init_parser(new_lexer("ls || cat Makefile &&"));
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser6);
    cr_assert(result6);
}


Test(parser, parser_multi_logical)          //marche
{
    struct parser *parser = init_parser(new_lexer("ls | echo test || cat test"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    struct parser *parser2= init_parser(new_lexer("ls || echo test || cat test"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);

    struct parser *parser3 = init_parser(new_lexer("ls && echo test && cat test"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);

    struct parser *parser4 = init_parser(new_lexer("ls | echo test && cat test | echo tata & cat tata"));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser4);
    cr_assert(!result4);

    struct parser *parser5 = init_parser(new_lexer("ls | echo test |"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser5);
    cr_assert(result5);

    struct parser *parser6 = init_parser(new_lexer("ls | > echo test"));
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser6);
    cr_assert(!result6);
}

Test(parser, parser_hard_test_simple_command){          // marche pas

    struct parser *parser = init_parser(new_lexer("/n"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

 
    struct parser *parser2 = init_parser(new_lexer("cd ../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../ ; pwd"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser2);
    cr_assert(!result2);


    struct parser *parser3 = init_parser(new_lexer("mkdir test1 ; cd test1 ; ls -a ; ls | cat | wc -c > fifi ; cat fifi"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser3);
    cr_assert(!result3);

    struct parser *parser4 = init_parser(new_lexer("echo \"No dollar character\" | cat -e"));  // répond connasse ! c'est bon je suis là wsh ca fais une heure je suis tout seul je regardais un film :o :o :o nana mais t'as grave gérer je sais plus écrire, en fait pour ce test la grammaire ne suit pas cest pour ca que ca marche pas du coup ? mdr oui je pense du coup psk je viens de check aussi la grammaire et nos fonctions donc oue , par contre faut qu'on test les not ! ah j'avoue 
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser4);
    cr_assert(!result4);
    
    struct parser *parser5 = init_parser(new_lexer(" ! echo \"No dollar character\" | cat -e"));  // répond connasse ! c'est bon je suis là wsh ca fais une heure je suis tout seul je regardais un film :o :o :o nana mais t'as grave gérer je sais plus écrire, en fait pour ce test la grammaire ne suit pas cest pour ca que ca marche pas du coup ? mdr oui je pense du coup psk je viens de check aussi la grammaire et nos fonctions donc oue , par contre faut qu'on test les not ! 
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser4);
    cr_assert(!result5);

    struct parser *parser6 = init_parser(new_lexer(" ! echo \"No dollar character\" | ! cat -e"));  // répond connasse ! c'est bon je suis là wsh ca fais une heure je suis tout seul je regardais un film :o :o :o nana mais t'as grave gérer je sais plus écrire, en fait pour ce test la grammaire ne suit pas cest pour ca que ca marche pas du coup ? mdr oui je pense du coup psk je viens de check aussi la grammaire et nos fonctions donc oue , par contre faut qu'on test les not ! 
    void *ast6 = NULL;
    bool result6 = parse_input(parser6, &ast6);
    // free_parser(parser4);
    cr_assert(result6);     //ici on check le not apres la pipeline qui ne doit pas marcher d'acc on est good on remballe ? ah oui ok c'est good, laisse le test quand même yes yes j'ai tellement mal au ventre mdr ils etaient enormes tes burgers aussi aussi enorme que tes bourlets mdr ct violen bru :'( x))) tkt je suis tjr aussi gentil voyons ptit fdp :oooooooooooooooooooooooooooo laisse ma mère trkl toi je vais te faire boire des shots juste pour vomir pétasse mdddddrrrr ramene les shots hahahahahahah dit elle elle a fini comme ca o-O   cpas drole jsuis vexée 
}

Test(parser, rule_for)
{
    struct parser *parser = init_parser(new_lexer("for i; do echo test done"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    struct parser *parser2 = init_parser(new_lexer("for i;\n\n\n do echo test done"));
    void *ast2 = NULL;
    bool result2 = parse_input(parser2, &ast2);
    // free_parser(parser);
    cr_assert(!result2);

    struct parser *parser3 = init_parser(new_lexer("for i in range;\n\n\n do echo test done"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser);
    cr_assert(!result3);

    struct parser *parser4 = init_parser(new_lexer("for i in range;\n\n\n do echo test; echo tata\necho toto done"));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser);
    cr_assert(!result4);

    struct parser *parser5 = init_parser(new_lexer("for i in range do echo test; echo tata\necho toto done"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser);
    cr_assert(result5);
}

Test(parser, funcdec)
{
    struct parser *parser = init_parser(new_lexer("function print_hello ( ) { echo hello }"));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    // free_parser(parser);
    cr_assert(!result);

    // A GERER DANS LE LEXER : parenthèses collées au nom de la fonction + parentheses collées entre elles
    // struct parser *parser2 = init_parser(new_lexer("function print_hello() { echo hello }"));
    // void *ast2 = NULL;
    // bool result2 = parse_input(parser2, &ast2);
    // // free_parser(parser);
    // cr_assert(!result2);

    struct parser *parser3 = init_parser(new_lexer("print_hello ( ) { echo hello }"));
    void *ast3 = NULL;
    bool result3 = parse_input(parser3, &ast3);
    // free_parser(parser);
    cr_assert(!result3);

    struct parser *parser4 = init_parser(new_lexer("print_hello ( ) { echo hello "));
    void *ast4 = NULL;
    bool result4 = parse_input(parser4, &ast4);
    // free_parser(parser);
    cr_assert(result4);

    struct parser *parser5 = init_parser(new_lexer("print_hello ( { echo hello }"));
    void *ast5 = NULL;
    bool result5 = parse_input(parser5, &ast5);
    // free_parser(parser);
    cr_assert(result5);
}