#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <wordexp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <err.h>
#include "commands.h"
#include "../parser/parser.h"
#include "./utils/xalloc.h"
#include "../exec/exec.h"
#include "../expansion/expansion.h"
#include "../storage/var_storage.h"
#include "../storage/program_data_storage.h"

extern char **environ;

/* args[0] = NULL for export and alias */

static void print_alias_storage()
{
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        if (!alias_storage->variables[i])
            continue;
        printf("alias %s=", alias_storage->variables[i]->key);
        printf("%s\n", alias_storage->variables[i]->value);
    }
}

void delete_alias(char **args)
{
    if (args[1] && args[1][0] == '-')
    {
        for (int i = 1; args[1][i]; i++)
        {
            if (args[1][i] != 'a')
            {
                warn("unalias: -%c: invalid option\n", args[1][i]);
                return;
            }
        }
        free_var_storage(alias_storage);
        alias_storage = new_var_storage();
    }
    else
    {
        if (!args[1])
            warn(UNALIAS_USAGE);
        else if (!del_var(alias_storage, args[1]))
            warn("unalias: %s: not found\n", args[1]);
    }
}

void create_alias(char **args)
{
    if (!args[0])
    {
        print_alias_storage(alias_storage);
        return;
    }
    if (args[0] && args[0][0] == '-')
    {
        for (int i = 1; args[0][i]; i++)
        {
            if (args[0][i] != 'p')
            {
                warn("alias: -%c: invalid option\n", args[0][i]);
                return;
            }
        }
        print_alias_storage(alias_storage);
    }
    int i = 0;
    if (args[0][0] == '-')
        i += 1;
    for (; args[i]; i++)
    {
        char *key = strsep(&args[i], "=");
        char *value = strsep(&args[i], "=");
        printf("key: %s\n value: %s\n", key, value);
        if (!value)
        {
            if (var_exists(alias_storage, key))
                printf("alias %s=%s\n", key, get_value(alias_storage, key));
            else
                warn("alias: %s: not found\n", key);
        }
        put_var(alias_storage, key, value);
    }
}

void load_file(char *path)
{
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        warn("%s: %s", path, strerror(errno));
        return;
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("%s", line);
        lexer = new_lexer(line);
        ast = parse(lexer);
        exec_node_input(ast);
    }
    fclose(fp);
}

void source(char **args)
{
    if (!args[1])
        warn("source: not enough arguments\n");
    char *path = NULL;
    if (!strstr(args[1], ".sh") && !strchr(args[1], '/'))
    {
        char *env = getenv("PATH");
        path = xmalloc(strlen(env) + strlen(args[1]) + 10);
        strcpy(path, getenv("PATH"));
        strcat(path, "/");
        strcat(path, args[1]);
    }
    for (int i = 2; args[i]; i++)
        append_program_data(args[i]);
    if (path == NULL)
        load_file(args[1]);
    else
        load_file(path);
}

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
        if (index_tab == 9)
            printf("%c", c[i]);
        i++;
    }
    return (0);
}

int	print_without_sp_madu(char *c)
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
            if (c[i] == '\\')
            {
                i++;
                if (c[i] == '\\')
                {
                    i++;
                    if (tab[index_tab].name == c[i])
                    {
                        printf("\\\\%c", tab[index_tab].corresp);
                        index_tab = 0;
                        i++;
                        break;
                    }
                }
                else
                {
                    if (tab[index_tab].name == c[i])
                    {
                        printf("%c", tab[index_tab].corresp);
                        index_tab = 0;
                        i++;
                        break;
                    }
                }
                // else if (c[i] != '\\')
                // {
                //     printf("%c", c[i]);
                //     i++;
                // }
            }
            index_tab++;
        }
        if (index_tab == 9)
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
            printf("%c", '\n');
    }
}

void echo(char **args)
{
    bool n = false;
    bool e = false;
    if (!args[1])
        printf("\n");
    else if (args[1][0] == '-' && args[1][1] != '\0')
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
        {
            update_last_status(0);
            return;
        }
        else 
            print_echo(args + 2, e, n);
    }
    else
        print_echo(args + 1, e, n);
    update_last_status(0);
}   

void cd(char **args)
{
    int ret = 0;
    char *pwd = getenv("PWD");
    if (!args[1])
        ret = chdir(getenv("HOME"));
    else if (strcmp(args[1], "-") == 0)
        ret = chdir(getenv("OLDPWD"));
    else
        ret = chdir(args[1]);
    char *new_pwd = xcalloc(1, 256);
    char *cwd = getcwd(NULL, 0);
    sprintf(new_pwd, "%s=%s", "PWD", cwd);
    putenv(new_pwd);
    free(cwd);
    if (ret == -1)
    {
        warn("%s", args[1]);
        update_last_status(1);
    }
    char *env = xcalloc(1, 256);
    sprintf(env, "%s=%s", "OLDPWD", pwd);
    putenv(env);
    update_last_status(0);
}

void export(char **args)
{
    bool p = false;
    bool n = false;
    if (!args[0])
    {
        for (char **env = environ; *env != NULL; env++)
            printf("declare -x %s\n", *env);
        update_last_status(0);
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
    {
        fprintf(stderr, "export: bad option\n");
        update_last_status(1);
    }
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
        {
            fprintf(stderr, "export: bad option\n");
            update_last_status(1);
        }
        for (int i = 1; args[i]; i++)
            unsetenv(args[i]);
    }
    else 
    {
        for (int i = 0; args[i]; i++)
        {
            putenv(args[i]);
        }
    }
    update_last_status(0);
}

void exit_shell(void)
{
    exit(atoi(program_data->last_cmd_status));
}

void func_continue(char **args)
{
    if (strcmp(args[0], "continue") == 0)
    {
        printf("bash: continue: only meaningful in a `for', `while', or `until' loop\n");
    }
    update_last_status(0);
}
