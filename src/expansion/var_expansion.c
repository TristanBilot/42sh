#include "expansion.h"
#include "../var_storage/var_storage.h"
#include "../utils/buffer.h"
#include <stdio.h>

size_t get_next_brack_index(const char *c, size_t j)
{
    if (!c)
        return 0;
    while (++j < strlen(c))
        if (c[j] == '}')
            return j;
    return -1;
}

char *perform_var_expansion(char *word)
{
    struct buffer *buf = new_buffer();
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (word[i] == '$')
        {
            i++;
            if (word[i] == '{')
            {
                i++;
                if (word[i] == '{') // ${{ is not an expansion
                    continue;
                int next_brack_idx = get_next_brack_index(word, i);
                if (next_brack_idx == -1)
                    continue;
                char *param = substr(word, i, next_brack_idx - i);
                if (var_exists(param))
                {
                    char *var = get_value(param);
                    for (size_t j = 0; j < strlen(var); j++)
                        append_buffer(buf, var[i]);
                    i = next_brack_idx;
                }
                else
                    break;
            }
            else
            {
                char *param = substr(word, i, strlen(word));
                if (var_exists(param))
                {
                    char *var = get_value(param);
                    for (size_t j = 0; j < strlen(var); j++)
                        if (!(j == strlen(var) - 1 && var[j] == '\n'))
                            append_buffer(buf, var[j]);
                    break;
                }
                else
                    break;
            }
        }
        else
            append_buffer(buf, word[i]);
    }
    append_buffer(buf, '\0');
    return buf->buf;
}
