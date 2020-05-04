#include <string.h>

#include "index_utils.h"

int is_separator(char c)
{
    return c == '\n' || c == ';' || c == '&';
}

size_t get_next_index(const char *str, char c, size_t i)
{
    if (!str)
        return 0;
    while (++i < strlen(str))
        if (str[i] == c)
            return i;
    return strlen(str);
}

size_t get_previous_index(const char *str, char c, size_t i)
{
    if (!str)
        return 0;
    while (i > 1 && str[--i])
        if (str[i] == c)
            return i;
    return 0;
}

size_t get_previous_separator_index(const char *str, size_t i)
{
    if (!str)
        return 0;
    while (i > 1 && str[--i])
        if (is_separator(str[i]))
            return i + 1;
    return 0;
}

size_t get_next_separator_index(const char *str, size_t i)
{
    if (!str)
        return 0;
    while (++i < strlen(str))
        if (is_separator(str[i]))
            return i;
    return strlen(str);
}

size_t get_next_close_curl_index(const char *str, size_t i)
{
    if (!str)
        return 0;
    while (++i < strlen(str))
        if (str[i] == '}')
            return i + 1;
    return strlen(str);
}
