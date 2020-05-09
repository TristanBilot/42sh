#include "history.h"
#include "../utils/xalloc.h"
#include <unistd.h>
#include <string.h>

struct history *open_history(void)
{
    struct history *history = xmalloc(sizeof(struct history));
    history->commands = NULL;
    history->index = 0;
    load_history(history);
    return history;
}

void append_history_command(struct history *history, char *cmd)
{
    // char *cmd_to_append = malloc(256);
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
    if (!f)
        perror("fopen error @ append history");
    fprintf(f, "%s\n", cmd);
    history->commands[history->index++] = cmd;
    fclose(f);
}

char *write_next_history(struct history *history)
{
    char *cmd = get_next_history(history);
    if (!cmd)
        return;
    // printf("==> %s$\n", cmd);
    // char *cmd = "ZZZZZtest";
    // for (size_t i = 0; i < strlen(cmd); i++)
    //     putchar(*cmd++);
    
    // write(0, cmd, strlen(cmd));
    
    // write_stdin(cmd);
    return cmd;
}

char *write_prev_history(struct history *history)
{
    char *cmd = get_prev_history(history);
    if (!cmd)
        return;
    // write_stdin(cmd);
    return cmd;
}

char *get_next_history(struct history *history)
{
    if (history->index - 1 < 0)
        return NULL;
    // for (int i = history->index; i >= 0; i--)
    //     printf("==> %s$\n", history->commands[i]);
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
    history->commands = xmalloc(sizeof(char *) * HISTORY_MAX);
    FILE *file = fopen(DEFAULT_HISTORY_FILE_NAME, "r");

    if (!file)
    {
        perror("fopen error @ history");
        return;
    }
    char *line = xmalloc(256);
    while (fgets(line, /*sizeof(line)*/256, file))
    {
        char *without_nl = xmalloc(256);
        for (int i = 0; line && (line[i] != '\n' && line[i] != '\0'); i++)
        {
            without_nl[i] = line[i];
        }
        history->commands[history->index++] = without_nl;
        line = xmalloc(256);
    }
    fclose(file);
}

void flush_stdin(void)
{
    // printf("flush\n");
    // fseek(stdin, 0, SEEK_END);
    // fflush(stdin);
    // int ch;
    // while (((ch = getchar()) != EOF) && (ch != 'X')) putchar(ch);
    // while (getchar() != EOF);
    // printf("AFTER");
    fflush(stdin);
}

void write_stdin(char *cmd)
{
    // flush_stdin();
    write(STDIN_FILENO, cmd, 256);
    printf("\n\ncmd: %s\n", cmd);
}

// void free_history(struct history *history)
// {
//     free(history->commands);
//     free(history);
// }
