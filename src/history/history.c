#include "history.h"
#include "../utils/xalloc.h"
#include <unistd.h>
#include <string.h>

struct history *open_history(void)
{
    struct history *history = xcalloc(1, sizeof(struct history));
    history->commands = NULL;
    history->index = 0;
    history->nb_lines = 0;
    load_history(history);
    return history;
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
    
    if (!is_only_spaces(cmd))
    {
        fprintf(f, "%s", cmd);
        history->commands[history->nb_lines++] = cmd;
        history->index++;
    }
    history->index = history->nb_lines;
    // printf("index : \n %d[", history->index);
    // for (int i = 0; i < history->nb_lines; i++)
    //     if (history->commands[i])
    //         printf("(%d) %p: %s, ", i, history->commands[i], history->commands[i]);
    // printf("]\n");
    fclose(f);
}

char *write_next_history(struct history *history)
{
    // printf("%d, %d\n", history->index, history->nb_lines);
    char *cmd = get_next_history(history);
    if (!cmd)
    {
        printf("cmd is null\n");
        return NULL;
    }
    return cmd;
}

char *write_prev_history(struct history *history)
{
    // printf("%d, %d\n", history->index, history->nb_lines);
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



void load_history(struct history *history)
{
    history->commands = xcalloc(1, sizeof(char *) * HISTORY_MAX);
    FILE *file = fopen(DEFAULT_HISTORY_FILE_NAME, "r");

    if (!file)
    {
        perror("fopen error @ history");
        return;
    }
    
    char *line = xcalloc(1, 256);
    while (fgets(line, /*sizeof(line)*/256, file))
    {
        //char *line = xcalloc(1, 256);
        char *without_nl = xcalloc(1, 256);
        for (int i = 0; line && (line[i] != '\n' && line[i] != '\0'); i++)
        {
            without_nl[i] = line[i];
        }
        history->commands[history->index++] = without_nl;
        history->nb_lines++;
        line = xcalloc(1, 256);
    }
    fclose(file);
}

// void flush_stdin(void)
// {
//     // printf("flush\n");
//         line = xcalloc(1, 256);
//     }
//     fclose(file);
// }

// void flush_stdin(void)
// {
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("fl
   // printf("flush\n");
   // fseek(stdin, 0, SEEK_END);
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
