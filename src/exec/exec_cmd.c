#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "exec.h"

void exec_element(char **args)
{
    int	status = 0;
    int	child = 0;
    if ((child = fork()) == -1)
        exit(1);
    if (child == 0)
    {
        if ((execvp(args[0], args)) == -1)
            err(1, "command not found: %s\n", args[0]);
        exit(0);
    }
    else
        wait(&status);
    if (WIFEXITED(status))
        printf("exit status = %d\n", WEXITSTATUS(status));
}

int main()
{
    char *args[2];
    args[0] = strdup("zzz");
    args[1] = strdup("-l");
    args[2] = NULL;
    exec_cmd(args);
    return 0;
}