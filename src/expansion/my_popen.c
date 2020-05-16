#include "my_popen.h"
#include "utils/xalloc.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

static pid_t pid = -1;
static FILE *global_stream = NULL;

FILE *my_popen(const char *cmd, const char *mode)
{
    int fds[2];
    int father;
    int child;

    if (global_stream)
    {
        errno = EAGAIN;
        return NULL;
    }

    if (!cmd)
    {
        errno = EINVAL;
        return NULL;
    }

    if (mode[1] != '\0')
    {
        errno = EINVAL;
        return NULL;
    }

    switch (mode[0])
    {
    case 'w':
        father = STDOUT_FILENO;
        child = STDIN_FILENO;
        break;
    case 'r':
        father = STDIN_FILENO;
        child = STDOUT_FILENO;
        break;
    default:
        errno = EINVAL;
        return NULL;
    }

    if (pipe(fds) == -1)
        return NULL;
    char *binary_42sh = xcalloc(1, 256);
    switch ((pid = fork()))
    {
    case -1:
        close(fds[father]);
        close(fds[child]);
        return NULL;
    case 0:
        close(fds[father]);
        if (fds[child] != child)
        {
            if (dup2(fds[child], child) == -1)
            {
                close(fds[child]);
            }
            close(fds[child]);
        }
        readlink("/proc/self/exe", binary_42sh, 256);
        execl(binary_42sh, binary_42sh, "-c", cmd, NULL);
        _exit(127);
    default:
        close(fds[child]);
        if ((global_stream = fdopen(fds[father], mode)) == NULL)
        {
            close(fds[father]);
            return NULL;
        }
        break;
    }
    return global_stream;
}

int my_pclose(FILE *stream)
{
    pid_t wait_pid;
    int status;

    if (!global_stream)
    {
        errno = ECHILD;
        return -1;
    }

    if (global_stream != stream)
    {
        errno = EINVAL;
        return -1;
    }

    if (fclose(stream) == EOF)
    {
        pid = -1;
        global_stream = NULL;
        return -1;
    }

    while ((wait_pid = waitpid(pid, &status, 0)) != pid)
    {
        if (wait_pid == -1)
        {
            if (errno == EINTR)
                continue;
            pid = -1;
            global_stream = NULL;
            return -1;
        }
    }
    pid = -1;
    global_stream = NULL;

    if (WIFEXITED(status) != 0)
        return WEXITSTATUS(status);

    errno = ECHILD;
    return -1;
}
