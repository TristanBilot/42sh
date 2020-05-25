#include "expansion.h"
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"
#include "../utils/buffer.h"
#include "../utils/index_utils.h"
#include "../utils/bracket_counter.h"
#include "../utils/my_itoa.h"
#include "./arithmetic/lexer/arithmetic_lexer.h"
#include "./arithmetic/ast/arithmetic_ast.h"
#include "./arithmetic/parser/arithmetic_parser.h"
#include "./arithmetic/exec/arithmetic_execution.h"
#include "my_popen.h"

char *perform_arithmetic_substitution(char *word)
{
    struct buffer *buf = new_huge_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (!(i < strlen(word) - 2 && word[i] == '$' && word[i + 1] == '('
            && word[i + 2] == '('))
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
        {
            FAIL_WITH_ERR("arithmetic lexer error")
        }
        struct arithmetic_ast *ast = NULL;
        if (parse_expression(lexer, &ast))
        {
            bool exec_error = false;
            double res = exec_arithmetic_ast(ast, &exec_error);
            if (exec_error)
            {
                FAIL_WITH_ERR("arithmetic execution error")
            }
            else
            {
                int to_integer = res;
                char *to_str = xmalloc(MAX_STR_LEN);
                to_str = my_itoa(to_integer, to_str);
                append_string_to_huge_buffer(buf, to_str);
                i += strlen(sub) + 1;
            }
        }
        else
        {
            FAIL_WITH_ERR("arithmetic parsing error")
        }
    }
    append_huge_buffer(buf, '\0');
    return buf->buf;
}
