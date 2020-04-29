#define _XOPEN_SOURCE
#include <wordexp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

extern char **environ;

void echo(char **args)
{
    args = args;
}

void cd(char **args)
{
    int ret = 0;
    if (!args[1])
        ret = chdir(getenv("HOME"));
    else if (strcmp(args[1], "-") == 0)
        ret = chdir(getenv("OLDPWD"));
    else
        ret = chdir(args[1]);
    if (ret == -1)
        printf("%s\n", strerror(errno));
}

void export(char **args)
{
    if (!args[1])
    {
        for (char **env = environ; *env != NULL; env++)
            printf("%s\n", *env);
    }
    else if (strcmp(args[1], "-p") == 0)
    {
        if (args[2])
            printf("%s\n", getenv(args[2]));
        else 
        {
            for (char **env = environ; *env != NULL; env++)
                printf("export %s\n", *env);
        }
    }
    else
    {
        putenv(args[1]);
    }
}

/*
void exit(char **args)
{

}
*/