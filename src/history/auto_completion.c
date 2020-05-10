#include <string.h>
#include "../utils/xalloc.h"
#include "history.h"

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

char *get_auto_completion(struct history *history, char *cmd)
{
    size_t len = strlen(cmd);
    size_t j = 0;
    int curr_score = 0;
    int best_score = 0;
    int best_idx = 0;
    char *curr = NULL;
    for (int i = 0; i < history->nb_commands; i++)
    {
        curr = xmalloc(len + 1);
        j = 0;
        for (; j < len; j++)
            curr[j] = history->commands[i][j];
        curr[j] = '\0';
        curr_score = levenshtein(cmd, strlen(cmd), curr, strlen(curr));
        if (curr_score > best_score)
        {
            best_score = curr_score;
            best_idx = i;
        }
    }
    printf("best_score: %d\n", best_score);
    printf("best_idx: %d\n", best_idx);
    return NULL;
}
