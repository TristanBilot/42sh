#include "expansion.h"
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"
#include "../utils/buffer.h"
#include "../utils/index_utils.h"
#include "my_popen.h"

char *perform_command_substitution(char *word)
{
    struct buffer *buf = new_huge_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (!(word[i] == '`' || (word[i] == '$' && ((word[i+1] && word[i+1] == '(') || !word[i+1]))))
        {
            append_huge_buffer(buf, word[i]);
            continue;
        }
        char *sub = NULL;
        if (word[i] == '$')
        { 
            i += 2;
            sub = substr(word, i, get_next_index(word, ')', i) - i);
        }
        else if (word[i] == '`')
        {
            i++;
            sub = substr(word, i, get_next_index(word, '`', i) - i);
        }
        FILE *out = my_popen(sub, "r");

        struct buffer *stdout_buf = new_huge_buffer();
        char ch;
        bool is_nl_printed = false;
        while ((ch = fgetc(out)) != EOF)
        {
            if (ch == '\n' || ch == ' ')
            {
                if (!is_nl_printed)
                    append_huge_buffer(stdout_buf, ' ');
                is_nl_printed = true;
            }
            else
            {
                is_nl_printed = false;
                append_huge_buffer(stdout_buf, ch);
            }
        }
        my_pclose(out);
        // printf("strlen: %zu\n", strlen(stdout_buf->buf));
        append_string_to_huge_buffer(buf, stdout_buf->buf);
        i += strlen(sub);
    }
    append_huge_buffer(buf, '\0');
    return buf->buf;
}
