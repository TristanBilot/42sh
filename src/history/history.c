#include "history.h"
#include "../utils/string_utils.h"
#include "../utils/xalloc.h"

struct history *open_history(void)
{
    struct history *history = xcalloc(1, sizeof(struct history));
    history->commands = NULL;
    history->index = 0;
    history->nb_lines = 0;
    load_history(history);
    return history;
}

void load_history(struct history *history)
{
    history->commands = xcalloc(1, sizeof(char *) * HISTORY_MAX);
    FILE *file = fopen(DEFAULT_HISTORY_FILE_NAME, "r");

    if (!file)
    {
        //perror("fopen error @ history");
        return;
    }
    
    char *line = xcalloc(1, MAX_STR_LEN);
    while (fgets(line, MAX_STR_LEN, file))
    {
        char *without_nl = xcalloc(1, MAX_STR_LEN);
        for (int i = 0; line && (line[i] != '\n' && line[i] != '\0'); i++)
            without_nl[i] = line[i];

        history->commands[history->index++] = without_nl;
        history->nb_lines++;
        line = xcalloc(1, MAX_STR_LEN);
    }
    fclose(file);
}

void append_history_command(struct history *history, char *cmd)
{
    // char *cmd_to_append = malloc(MAX_STR_LEN);
    // int i = 0;
    // int j = 0;
    // while (cmd && cmd[i++])
    // {
    //     if (cmd[i] == '\n')
    //     {
    //         cmd_to_append[j++] = ';';
    //         cmd_to_append[j++] = ' ';
    //     }
    //     else
    //         cmd_to_append[j++] = cmd[i];
    // }
    // cmd_to_append[--j] = '\0';
    // cmd_to_append[--j] = '\0';
    FILE *f = fopen(DEFAULT_HISTORY_FILE_NAME, "a");
    //if (!f)
    //    perror("fopen error @ append history");
    
    if (!is_only_spaces(cmd))
    {
        fprintf(f, "%s\n", cmd);
        history->commands[history->nb_lines++] = cmd;
        history->index++;
    }
    history->index = history->nb_lines;
    fclose(f);
}

char *write_next_history(struct history *history)
{
    char *cmd = get_next_history(history);
    if (!cmd)
        return NULL;
    return cmd;
}

char *write_prev_history(struct history *history)
{
    char *cmd = get_prev_history(history);
    if (!cmd)
        return NULL;
    return cmd;
}

char *get_next_history(struct history *history)
{
    if (history->index - 1 < 0)
        return NULL;
    return history->commands[--history->index];
}

char *get_prev_history(struct history *history)
{
    if (history->index + 1 > history->nb_lines)
        return NULL;
    return history->commands[++history->index];
}

bool is_only_spaces(char *cmd)
{
    if (strlen(cmd) == 0 || cmd[0] == '\n')
        return true;
    for (size_t i = 0; i < strlen(cmd); i++)
        if (cmd[i] != ' ' && cmd[i] != '\n')
            return false;
    return true;
}
