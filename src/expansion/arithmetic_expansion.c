#include "expansion.h"
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"
#include "../utils/buffer.h"
#include "../utils/index_utils.h"
#include "../utils/bracket_counter.h"
#include "./arithmetic/lexer/arithmetic_lexer.h"
#include "my_popen.h"

char *perform_command_substitution(char *word)
{
    struct buffer *buf = new_huge_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (!(i < strlen(word) - 2 && word[i] == '$' && word[i + 1] == '(' && word[i + 2] == '('))
            continue;
        
        int closing_parent = get_closing_parent_index(word, i + 1);
        if (closing_parent == -1)
            return NULL;
        i += 3;
        char *sub = substr(word, i, closing_parent - i);
        struct arithmetic_lexer *lexer = new_arithmetic_lexer(sub);
        if (!lexer)
            return NULL;
        
    }
    append_huge_buffer(buf, '\0');
    return buf->buf;
}
