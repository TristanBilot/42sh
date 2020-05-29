#include <stdlib.h>
#include <stdio.h>
#include "program_data_storage.h"
#include "../utils/string_utils.h"

struct program_data_storage *program_data;
void new_program_data_storage(int argc, char *argv[])
{
    program_data = malloc(sizeof(struct program_data_storage));
    program_data->last_cmd_status = calloc(MAX_STR_LEN, 1);
    program_data->binary_name = argv ? argv[0] : NULL;
    if (argc <= 1)
    {
        program_data->argv = NULL;
        program_data->argc = 0;
        return;
    }
    char **args = calloc(argc - 1, sizeof(char *));
    for (int i = 1; i < argc; i++)
        args[i - 1] = argv[i];
    program_data->argv = args;
    program_data->argc = argc - 1;
}

void append_program_data(char *element)
{
    if (program_data->argv == NULL)
        program_data->argv = calloc(1, sizeof(char *));
    else
        program_data->argv = realloc(program_data->argv, sizeof(char *) + 1);
    int i = program_data->argc;
    program_data->argv[i] = element;
    program_data->argc += 1;
}

void free_program_data_storage(void)
{
    if (program_data->argv)
        free(program_data->argv);
    free(program_data->last_cmd_status);
    free(program_data);
}

void update_last_status(int status)
{
    sprintf(program_data->last_cmd_status, "%d", status);
}
