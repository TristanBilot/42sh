#define _XOPEN_SOURCE
#include <wordexp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<signal.h>
#include <stdbool.h>
#include "commands.h"

extern char **environ;

/*void sig_usr(int signo)
{
    if(signo == SIGINT)
    printf("Signal caught!");
    return;
}*/

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

int	print_without_sp(char *c)
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
                printf("%c", tab[index_tab].corresp);
                index_tab = 0;
                i++;
                break;
            }
            index_tab++;
        }
        if (index_tab == 10)
        printf("%c", c[i]);
        i++;
    }
    return (0);
}

void print_echo(char **args, bool e, bool n)
{
    if (e == false && n == false)
    {
        for (int i = 0; args[i]; i++)
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf("%c", ' ');
        }
        printf("\n");
    }
    else if (n == true && e == false)
    {
        for (int i = 0; args[i]; i++)
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf("%c", ' ');
        }
    }
    else if (e == true)
    {
        for (int i = 0; args[i]; i++)
        {
            print_without_sp(args[i]);
            if (args[i + 1])
                printf("%c", ' ');
        }
        if (n == false)
        {
            printf("%c", '\n');
        }
    }
}

void echo(char **args)
{
    bool n = false;
    bool e = false;
    if (!args[1])
        printf("\n");
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
            print_echo(args + 1, false, false);
        else if (!args[2])
            return;
        else 
            print_echo(args + 2, e, n);
    }
    else
        print_echo(args + 1, e, n);
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

void exit_shell(char **args)
{
    (void)args;
    //if (signal(SIGINT, sig_usr) == SIG_ERR)
    //    printf("Signal processed ");
}