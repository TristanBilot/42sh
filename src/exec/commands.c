#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <wordexp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <err.h>
#include "commands.h"

extern char **environ;

static void fill_echo_tab(struct echo_tab tab[10])
{
    tab[0].name = 'n';
    tab[0].corresp = '\n';
    tab[1].name = '\\';
    tab[1].corresp = '\\';
    tab[2].name = 'a';
    tab[2].corresp = '\a';
    tab[3].name = 'b';
    tab[3].corresp = '\b';
    tab[4].name = 'c';
    tab[4].corresp = '\0';
    //tab[5].name = 'e';
    //tab[5].corresp = '\e';
    tab[5].name = 'f';
    tab[5].corresp = '\f';
    tab[6].name = 'r';
    tab[6].corresp = '\r';
    tab[7].name = 't';
    tab[7].corresp = '\t';
    tab[8].name = 'v';
    tab[8].corresp = '\v';
}

void print_args(char **args)
{
    int i = 0;
    while (args[i])
    {
        printf("args %d : %s\n", i, args[i]);
        i++;
    }
}
int	print_without_sp(char *c, int *ptr_fd)
{
    int i = 0;
    struct echo_tab tab[9];
    int index_tab = 0;

    fill_echo_tab(tab);
    while (c[i] != '\0')
    {
        index_tab = 0;
        while (index_tab < 9)
        {
            if (c[i] == '\\' && tab[index_tab].name == c[i + 1])
            {
                dprintf(*ptr_fd, tab[index_tab].corresp);
                //printf("%c", tab[index_tab].corresp);
                index_tab = 0;
                i++;
                break;
            }
            index_tab++;
        }
        if (index_tab == 10)
            dprintf(*ptr_fd, c[i]);
            //printf("%c", c[i]);
        i++;
    }
    return (0);
}

void print_echo(char **args, bool e, bool n, int *ptr_fd)
{
    if (e == false && n == false)
    {
        for (int i = 0; args[i]; i++)
        {
            dprintf(*ptr_fd, args[i]);
            //printf("%s", args[i]);
            if (args[i + 1])
                dprintf(*ptr_fd, " ");
                //printf("%c", ' ');
        }
        dprintf(*ptr_fd, "\n");
        //printf("\n");
    }
    else if (n == true && e == false)
    {
        for (int i = 0; args[i]; i++)
        {
            dprintf(*ptr_fd, args[i]);
            // printf("%s", args[i]);
            if (args[i + 1])
                dprintf(*ptr_fd, " ");
                //printf("%c", ' ');
        }
    }
    else if (e == true)
    {
        for (int i = 0; args[i]; i++)
        {
            print_without_sp(args[i], ptr_fd);
            if (args[i + 1])
                dprintf(*ptr_fd, args[i]);
                //printf("%c", ' ');
        }
        if (n == false)
        {
            dprintf(*ptr_fd, "\n");
            //printf("%c", '\n');
        }
    }
}

void echo(char **args, int *ptr_fd)
{
    bool n = false;
    bool e = false;
    if (!args[1])
        dprintf(*ptr_fd, "\n");
        //printf("\n");
    else if (args[1][0] == '-')
    {
        int i = 0;
        for (i = 1; (args[1][i] == 'n' || args[1][i] == 'e') && args[1][i]; i++)
        {
            if (args[1][i] == 'n')
                n = true;
            if (args[1][i] == 'e')
                e = true;
        }
        if (args[1][i] != 0)
            print_echo(args + 1, false, false, ptr_fd);
        else if (!args[2])
            return;
        else 
            print_echo(args + 2, e, n, ptr_fd);
    }
    else
        print_echo(args + 1, e, n, ptr_fd);
}   

void cd(char **args, int *ptr_fd)
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

void export(char **args, int *ptr_fd)
{
    bool p = false;
    bool n = false;
    if (!args[0])
    {
        for (char **env = environ; *env != NULL; env++)
            printf("declare -x %s\n", *env);
        return;
    }
    else if (args[0][0] ==  '-')
    {
        for (int i = 1; args[0][i] && (args[0][i] == 'p' || args[0][i] == 'n'); i++)
        {
            if (args[0][i] == 'p')
                p = true;
            if (args[0][i] == 'n')
                n = true;
        }
    }
    if (p && n)
        err(1, "export: bad option\n");
    else if (p)
    {
        if (args[1])
        {
            printf("declare -x %s\n", getenv(args[1]));
        }
        else
        {
            for (char **env = environ; *env != NULL; env++)
                printf("declare -x %s\n", *env);
        }
    }
    else if (n)
    {
        if (!args[1])
            err(1, "export: bad option\n");
        for (int i = 1; args[i]; i++)
            unsetenv(args[i]);
    }
    else 
    {
        for (int i = 0; args[i]; i++)
            putenv(args[i]);
    }
}

void exit_shell(char **args)
{
    if (args[0])
    {
        if (args[1])
            err(1, "exit: too many arguments\n");
        exit(atoi(args[1]));
    }
    else
        exit(0);
}