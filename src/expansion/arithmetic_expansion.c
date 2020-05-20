#include "expansion.h"
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"
#include "../utils/buffer.h"
#include "../utils/index_utils.h"
#include "../utils/bracket_counter.h"
#include "./arithmetic/lexer/arithmetic_lexer.h"
#include "./arithmetic/ast/arithmetic_ast.h"
#include "./arithmetic/parser/arithmetic_parser.h"
#include "./arithmetic/eval/arithmetic_ast_print.h"
#include "./arithmetic/exec/arithmetic_execution.h"
#include "my_popen.h"

char *perform_arithmetic_substitution(char *word)
{
    struct buffer *buf = new_huge_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (!(i < strlen(word) - 2 && word[i] == '$' && word[i + 1] == '(' && word[i + 2] == '('))
        {
            append_huge_buffer(buf, word[i]);
            continue;
        }
        int closing_parent = get_closing_parent_index(word, i + 1);
        if (closing_parent == -1)
            return NULL;
        i += 3;
        char *sub = substr(word, i, closing_parent - i - 1);
        struct arithmetic_lexer *lexer = new_arithmetic_lexer(sub);
        if (!lexer)
            return NULL;
        // struct arithmetic_token *token = NULL;
        struct arithmetic_ast *ast = NULL;
        // while ((token = pop_arithmetic(lexer)))
        //     printf("%s %d\n", token_str(token->type), token->value);
        
        if (parse_expression(lexer, &ast))
        {
            // printf("--- AST ---\n");
            // print_arithmetic_ast(ast);
            // printf("\n--- END ---\n");

            bool *exec_error = false;
            double res = exec_arithmetic_ast(ast, exec_error);
            if (exec_error)
                printf("arithmetic execution error\n");
            else
            {
                int to_integer = res;
                printf("result of arithmetic: %d\n", to_integer);
            }
        }
        else
            puts("Arithmetic parsing error");
    }
    append_huge_buffer(buf, '\0');
    return buf->buf;
}
