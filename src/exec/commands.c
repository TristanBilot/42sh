#include <wordexp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void echo(char **args)
{
    args = args;
    // printf(args[1]);
}

void cd(char **args)
{
    if (chdir(args[1]) == -1)
        printf("%s\n", strerror(errno));
}

void export(char **args)
{
    args = args;
    // wordexp_t p;
    // putenv(args[1]);
}