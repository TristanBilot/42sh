#include <criterion/criterion.h>
#include "../expansion/arithmetic/lexer/arithmetic_lexer.h"
#include "../garbage_collector/garbage_collector.h"
#include <stdio.h>
#include <stdbool.h>

struct var_storage *alias_storage;
struct var_storage *var_storage;
struct file_manager *file_manager;

bool should_fail(char *exp)
{
    new_garbage_collector();
    struct arithmetic_lexer *lexer = new_arithmetic_lexer(exp);
    bool res = lexer == NULL;
    free_garbage_collector();
    free(garbage_collector);
    return res;
}

void print_tokens(char *exp)
{
    new_garbage_collector();
    struct arithmetic_lexer *lexer = new_arithmetic_lexer(exp);
    if (!lexer)
        fprintf(stderr, "%s", "lexer error\n");
    else
    {
        struct arithmetic_token *token = NULL;
        while ((token = pop_arithmetic(lexer)))
            printf("%s %d\n", token_str(token->type), token->value);
    }
    free_garbage_collector();
    free(garbage_collector);
}

Test(arithmetic_lexer, basic)
{
    new_garbage_collector();
    char *str = "1+2+3";
    struct arithmetic_lexer *lexer = new_arithmetic_lexer(str);
    //  struct arithmetic_token *token = NULL;
    //     while ((token = pop_arithmetic(lexer)))
    //         printf("%s %d\n", token_str(token->type), token->value);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 1);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_PLUS);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 2);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_PLUS);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    // fprintf(stderr, "==>%s\n", token_str(peek_arithmetic(lexer)->type));
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 3);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_END);
    free_garbage_collector();
    free(garbage_collector);
}

Test(arithmetic_lexer, medium)
{
    new_garbage_collector();
    char *str = "((90*1300) - (0/4) + 2)";
    struct arithmetic_lexer *lexer = new_arithmetic_lexer(str);
    //  struct arithmetic_token *token = NULL;
    //     while ((token = pop_arithmetic(lexer)))
    //         printf("%s %d\n", token_str(token->type), token->value);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_LPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_LPAR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 90);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_MULTIPLY);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 1300);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_RPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_MINUS);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_LPAR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_DIVIDE);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 4);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_RPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_PLUS);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 2);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_RPAR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_END);
    free_garbage_collector();
    free(garbage_collector);
}

Test(arithmetic_lexer, hard)
{
    new_garbage_collector();
    // char *str = "!((47&&20||0)|1&3^44~33)   **4622";
    char *str = "    !((47&&20||0)|1&3^44~33)   **4622   ";
    struct arithmetic_lexer *lexer = new_arithmetic_lexer(str);
    //  struct arithmetic_token *token = NULL;
    //     while ((token = pop_arithmetic(lexer)))
    //         printf("%s %d\n", token_str(token->type), token->value);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_NOT);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_LPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_LPAR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 47);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_AND);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 20);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_OR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 0);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_RPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_PIPE);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 1);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_SEPAND);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 3);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_XOR);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 44);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_TILDE);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 33);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_RPAR);
    cr_assert(pop_arithmetic(lexer)->type == TOK_A_POW);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_NUMBER);
    cr_assert(pop_arithmetic(lexer)->value == 4622);
    cr_assert(peek_arithmetic(lexer)->type == TOK_A_END);
    free_garbage_collector();
    free(garbage_collector);
}

Test(arithmetic_lexer, fails)
{
    cr_assert(should_fail("1+()"));
    cr_assert(should_fail("/2"));
    cr_assert(should_fail("*2"));
    cr_assert(should_fail("2^~3"));
    cr_assert(should_fail("*2"));
    cr_assert(should_fail("*"));
    cr_assert(should_fail("test"));
    cr_assert(should_fail("((2+3)"));
    cr_assert(should_fail("((2+3)))"));
    cr_assert(should_fail("3*/2"));
}

Test(arithmetic_lexer, successes)
{
    cr_assert(!should_fail("3*+20"));
    cr_assert(!should_fail("3*-(20)"));
    cr_assert(!should_fail("(((3*9-+6)))"));
    cr_assert(!should_fail("1++2"));
    cr_assert(!should_fail("1+-2"));
}
