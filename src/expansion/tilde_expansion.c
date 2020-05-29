#include "expansion.h"
#include "../utils/buffer.h"

char *perform_tilde_expansion(char *word)
{
    struct buffer *buf = new_buffer();
    char *sub = NULL;
    // bool forbid_expansion = false;
    for (size_t i = 0; i < strlen(word); i++)
    {
        // if (word[i] == '\'')
        //     forbid_expansion = !forbid_expansion;
        if (/*!forbid_expansion &&*/ word[i] == '~' && i == 0)
        {
            i++;
            if (word[i])
            {
                if (word[i] == '+'
                    && (sub = substitute_plus_tilde(word, &i)))
                    append_string_to_buffer(buf, sub);
                else if (word[i] == '-'
                && (sub = substitute_minus_tilde(word, &i)))
                    append_string_to_buffer(buf, sub);
                else if ((sub = substitute_tilde(word, &i)))
                    append_string_to_buffer(buf, sub);
                else
                    append_buffer(buf, '~');
            }
            else
                append_string_to_buffer(buf, substitute_tilde(word, &i));
            i--;
        }
        else
            append_buffer(buf, word[i]);
    }
    append_buffer(buf, '\0');
    return buf->buf;
}

bool is_valid_tilde(char *word, size_t i)
{
    if (word[i] && word[i] == '/')
        return true;
    return (!word[i] || ((word[i] == '+' || word[i] == '-') \
        && (!word[i + 1] || word[i + 1] == '/')));
}

char *substitute_minus_tilde(char *word, size_t *i)
{
    if (!is_valid_tilde(word, *i))
        return NULL;

    (*i)++;
    char *old_pwd = getenv("OLDPWD");
    return old_pwd ? old_pwd : NULL;
}

char *substitute_plus_tilde(char *word, size_t *i)
{
    if (!is_valid_tilde(word, *i))
        return NULL;

    (*i)++;
    char *pwd = getenv("PWD");
    return pwd ? pwd : NULL;
}

char *substitute_tilde(char *word, size_t *i)
{
    if (!is_valid_tilde(word, *i))
        return NULL;

    char *home = getenv("HOME");
    return home ? home : NULL;
}
