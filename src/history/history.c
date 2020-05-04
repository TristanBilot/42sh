#include "history.h"

struct history *open_history(void)
{
    struct history *history = malloc(sizeof(struct history));
    history->commands = NULL;
    history->index = 0;
    load_history(history);
    return history;
}

void append_history_command(struct history *history, char *cmd)
{
    FILE *f = fopen(DEFAULT_HISTORY_FILE_NAME, "a");
    if (!f)
        perror("fopen error @ append history");
    fprintf(f, "%s\n", cmd);
    history->commands[history->index++] = cmd;
    fclose(f);
}

void write_next_history(struct history *history)
{
    char *cmd = get_next_history(history);
    if (!cmd)
        return;
    flush_stdin();
    write_stdin(cmd);
}

void write_prev_history(struct history *history)
{
    char *cmd = get_prev_history(history);
    if (!cmd)
        return;
    flush_stdin();
    write_stdin(cmd);
}

char *get_next_history(struct history *history)
{
    if (history->index - 1 <= 0)
        return NULL;
    return history->commands[--history->index];
}

char *get_prev_history(struct history *history)
{
    if (history->index + 1 > HISTORY_MAX)
        return NULL;
    return history->commands[++history->index];
}

void load_history(struct history *history)
{
    history->commands = malloc(sizeof(char *) * HISTORY_MAX);
    FILE *file = fopen(DEFAULT_HISTORY_FILE_NAME, "r");

    if (!file)
    {
        perror("fopen error @ history");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
        history->commands[history->index++] = line;
    fclose(file);
}

void flush_stdin(void)
{
    fseek(stdin, 0, SEEK_END);
}

void write_stdin(char *cmd)
{
    flush_stdin();
    fprintf(stdin, "%s", cmd);
}

void free_history(struct history *history)
{
    free(history->commands);
    free(history);
}
