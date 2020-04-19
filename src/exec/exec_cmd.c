#include "exec.h"

void exec_cmd()
{
    int	status = 0;
    int	child = 0;
    if ((child = fork()) == -1)
        exit(1);
    if (child == 0)
    {
        
        exit(0);
    }
    else
        wait(&status);
    if (WIFEXITED(res))
        printf("exit status = %d\n", WEXITSTATUS(res))
}