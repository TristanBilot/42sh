#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/string.h"

bool success(const char *expr)
{
    struct parser *parser = init_parser(new_lexer(expr));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    return !result;
}

bool fail(const char *expr)
{
    struct parser *parser = init_parser(new_lexer(expr));
    void *ast = NULL;
    bool result = parse_input(parser, &ast);
    return result;
}

Test(parser, parse_redirection)
{
    cr_assert(success("1>2"));
    cr_assert(success("1>2"));
    cr_assert(success("1<2"));
}

Test(parser, more_redirection)
{
    cr_assert(success("1<>2"));
    cr_assert(success("1>&2"));
    cr_assert(success("1>>2"));
    cr_assert(success("1<<2"));
    cr_assert(success("1<<-2"));
    cr_assert(success("1>|2"));
    cr_assert(success("1<&2"));
}

Test(parser, parse_simple_command)
{
    cr_assert(success("ls"));
    cr_assert(success("echo test"));
}

Test(parser, parser_assigment_word)
{
    cr_assert(success("a=1"));
    cr_assert(success("a=1 echo a"));
}

Test(parser, parser_simple_command2){

    cr_assert(success("\n"));
    cr_assert(success(" "));
    cr_assert(success("ls;echo toto"));
    cr_assert(success(""));
    cr_assert(success("ls;echo tototo;ls"));
    cr_assert(success("ls;  echo toto;ls"));
    cr_assert(success("ls;\n"));
    cr_assert(fail("ls;\n\necho toto;ls\n"));                           
    cr_assert(fail("ls \n echo toto \n ls\n"));
}

Test(parser, parse_simple_if)
{
    cr_assert(success("if a then b fi"));
    //cr_assert(success("if a then b fi\n"));
    cr_assert(success("if a then b else c fi"));
    cr_assert(success("if a then b elif c then d else e fi"));
    cr_assert(success("if a then b elif c then d fi"));
    cr_assert(success("if a then b elif c then d elif e then f else g fi"));
    cr_assert(fail("if a then b  c then d elif e then f else g fi"));
    cr_assert(fail("if a then b else c"));
}

Test(parser, parser_and_or_simple)
{
    cr_assert(success("ls | cat test"));
    cr_assert(success("ls | cat test")); 
    cr_assert(success("ls & cat Makefile"));
    cr_assert(success("ls && cat Makefile"));
    cr_assert(fail("ls && || cat Makefile"));
    cr_assert(fail("ls || cat Makefile &&"));
}

Test(parser, parser_multi_logical)          
{
    cr_assert(success("ls | echo test || cat test"));
    cr_assert(success("ls || echo test || cat test"));
    cr_assert(success("ls && echo test && cat test"));
    cr_assert(success("ls | echo test && cat test | echo tata & cat tata"));
    cr_assert(success("ls | > echo test"));
    cr_assert(fail("ls | echo test |"));
}

Test(parser, parser_hard_test_simple_command)
{
    cr_assert(success("/n"));
    cr_assert(success("cd ../../../../../../../../../../../../../../../../../../../ ; pwd"));
    cr_assert(success("mkdir test1 ; cd test1 ; ls -a ; ls | cat | wc -c > fifi ; cat fifi"));
    cr_assert(success("echo \"No dollar character\" | cat -e"));
    cr_assert(success(" ! echo \"No dollar character\" | cat -e"));
    cr_assert(fail(" ! echo \"No dollar character\" | ! cat -e"));
}

Test(parser, rule_for)
{
    cr_assert(success("for i; do echo test done"));
    cr_assert(success("for i;\n\n\n do echo test done"));
    cr_assert(success("for i in range;\n\n\n do echo test done"));
    cr_assert(success("for i in range;\n\n\n do echo test; echo tata\necho toto done"));
    cr_assert(fail("for i in range do echo test; echo tata\necho toto done"));
}

Test(parser, rule_while)
{
    cr_assert(success("while a + b do echo toto done"));
    cr_assert(success("while ( a + b ) do echo toto done"));
    cr_assert(success("while ( a+b ) do echo toto done"));
    cr_assert(success("while a+b && ( 2 * 3 )  do echo toto done"));
    cr_assert(fail("while a + b do echo toto"));
    cr_assert(fail("while a + b echo toto done"));
    cr_assert(fail("while a + b echo toto done"));
    cr_assert(fail("while do echo toto done"));
}

Test(parser, funcdec)
{
    cr_assert(success("function print_hello ( ) { echo hello }"));
    cr_assert(success("function print_hello() { echo hello }"));
    cr_assert(success("print_hello ( ) { echo hello }"));
    cr_assert(fail("print_hello ( ) { echo hello "));
    cr_assert(fail("print_hello ( { echo hello }"));
}

Test(parser, parenthesis)
{
    cr_assert(success("( a && b ) || c"));
    cr_assert(success("( a || ( b && c || ls ) )"));
    cr_assert(success("( a || ( ( b && c ) || ls ) )"));
    cr_assert(success("a && ( b )"));
    cr_assert(fail("a && ( b"));
    cr_assert(fail("( a || ( ( ) b && c ) || ls )"));
    cr_assert(fail("( a || ( b && c ) || ls ) )"));
}
Test(parser, rule_until)
{
    cr_assert(success("until a + b do echo toto done"));
    cr_assert(success("until ( a+b ) do echo toto done"));
    cr_assert(success("until ( a+b && ( 2 * 3 ) ) do echo toto done"));
    cr_assert(success("until ( a + b ) do echo toto done"));                    
    cr_assert(fail("\nuntil a + b do echo toto"));  
    cr_assert(fail("until a + b  echo toto done"));
    cr_assert(fail("until a + b echo toto done"));
    cr_assert(fail("until do echo toto done"));
}

Test(parser, rule_case)
{
    cr_assert(success("case a in esac"));
    cr_assert(success("case a in b ) hello ;; esac"));
    cr_assert(success("case a in b | c ) hello ;; esac"));
    cr_assert(success("case a in b | c | d ) hello ;; esac"));
    cr_assert(success("case a in b ) hello ;; c ) toto ;; d ) yolo ;; esac"));
    cr_assert(success("case str in hello | totoro ) echo hello ;; bye | totolo ) echo test ;; esac"));
    cr_assert(fail("case a in ) hello ;; esac"));
    cr_assert(fail("case a in ) hello ; esac"));
    cr_assert(fail("case a in b c) hello ;; esac"));
    cr_assert(fail("case a in b | c | ) hello ;; esac"));
    cr_assert(fail("case str in hello | totoro ) echo hello ;; bye | totolo ) echo test ;; "));
}
Test(parser, hardcore_test)         // erreur sur les ; -> a corriger ;-> doit etre considerer comme un WORD
{
    cr_assert(success("! case str in hello | totoro ) echo hello ;; bye | totolo ) echo test ;; esac | function print ( ) { for i in range 1 2 ; do echo test ; ( if a then b elif c && j then d else e fi ) ; done ; ls } 1>&2"));
    cr_assert(success("while a > b do case res in 1 )  cd ../../../../../../../../../../../../../../../../../../../ ; pwd ;; 2 ) until a + b do echo hello world done ;; 3 ) for i in range;\n\n\n do echo test; echo tata\necho toto done ;; 4) if a then b elif c then d elif e then f else g fi ;; 5 ) ls | echo test && cat test | echo tata & cat tata ;; 6 ) a=1 echo a ;; esac done"));
}

Test(parser, parenthesis_near)
{
    cr_assert(success("until (a+b) do echo toto done"));
    cr_assert(success("case a in b | c) hello ;; esac"));
    cr_assert(success("case str in hello | totoro) echo hello ;; bye | totolo) echo test ;; esac"));
    cr_assert(success("(a && b) || c"));
    cr_assert(success("print_hello () { echo hello }"));
    cr_assert(success("print_hello() { echo hello }"));
    cr_assert(success("print_hello() { echo hello } | hello caca"));
    cr_assert(success("! case str in hello | totoro) echo hello ;; bye | totolo) echo test ;; esac | function print() { for i in range 1 2 ; do echo test ; ( if a then b elif c && j then d else e fi ) ; done ; ls } 1>&2")); 
    cr_assert(success("(if a then b elif c && j then d else e fi)"));
}

Test(parser, comments)
{
    cr_assert(success("#commentaire  ter"));   //marche sur le terminal
    cr_assert(success("# commentaire"));  // marche aussi sur le terminal
    // Marche car le lexer prend bien ls#commentaire comme un WORD => le parser accepte les simple word => ne marchera pas à l'exécution
    //cr_assert(fail("ls#commentaire")); // pas censé marcher ça marche pas sur le terminal donc mis en fail
    cr_assert(success("ls # commentaire")); // ça marche sur le terminal
    cr_assert(success("ls;#commentaire")); // ça marche sur le terminal
    cr_assert(fail("#commentaire\necho test"));
}