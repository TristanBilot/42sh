#include <string.h>
#include "../utils/xalloc.h"
#include "history.h"

/*
* Good for real auto-complete, could be use in a second time after
* classic dist_algorithm.
*/
int levenshtein(const char *s, int len1, const char *t, int len2)
{
    int a, b, c;
    if (!len1)
        return len2;
    if (!len2)
        return len1;

    if (s[len1 - 1] == t[len2 - 1])
        return levenshtein(s, len1 - 1, t, len2 - 1);

    a = levenshtein(s, len1 - 1, t, len2 - 1);
    b = levenshtein(s, len1, t, len2 - 1);
    c = levenshtein(s, len1 - 1, t, len2);

    if (a > b)
        a = b;
    if (a > c)
        a = c;
    return a + 1;
}

/*
* Better for a bash-like auto-completing.
*/
bool dist_algorithm(const char *s, int len1, const char *t, int len2)
{
    int longest = len1 > len2 ? len1 : len2;
    for (int i = 0; i < longest; i++)
        if (s[i] != t[i])
            return false;
    return true;
}

char *get_auto_completion(struct history *history, char *cmd)
{
    size_t len = strlen(cmd);
    size_t j = 0;
    int best_idx = INF;
    char *curr = NULL;

    for (int i = history->index - 1; i >= 0; i--)
    {
        curr = xcalloc(1, len + 1);
        j = 0;
        for (; history->commands[i] && j < strlen(history->commands[i]) && j < len; j++)
            curr[j] = history->commands[i][j];
        curr[j] = '\0';

        if (dist_algorithm(cmd, strlen(cmd), curr, strlen(curr)))
        {
            best_idx = i;
            break;
        }
    }
    return best_idx == INF ? NULL : history->commands[best_idx];
}
