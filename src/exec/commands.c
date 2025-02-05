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


/* args[0] = NULL for export and alias */

static void print_alias_storage()
{
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        if (!alias_storage->variables[i])
            continue;
        printf("%s=", alias_storage->variables[i]->key);
        printf("\'%s\'\n", alias_storage->variables[i]->value);
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
                update_last_status(2);
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
    update_last_status(0);
}

void create_alias(char **args)
{
    if (!args[0])
    {
        print_alias_storage();
        update_last_status(0);
        return;
    }
    if (args[0] && args[0][0] == '-')
    {
        for (int i = 1; args[0][i]; i++)
        {
            if (args[0][i] != 'p')
            {
                warn("alias: -%c: invalid option\n", args[0][i]);
                update_last_status(2);
                return;
            }
        }
        print_alias_storage();
    }
    int i = 0;
    if (args[0][0] == '-')
        i += 1;
    for (; args[i]; i++)
    {
        char *key = strsep(&args[i], "=");
        char *value = strsep(&args[i], "=");
        if (!value)
        {
            if (var_exists(alias_storage, key))
                printf("%s=%s\n", key,
                    get_value(alias_storage, key));
            else
                warn("alias: %s: not found\n", key);
        }
        put_var(alias_storage, key, value);
    }
    update_last_status(0);
}

bool load_file(char *path, bool warning)
{
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        (void) warning;
        return false;
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        lexer = new_lexer(line);
        ast = parse(lexer);
        exec_node_input(ast);
    }
    free(line);
    fclose(fp);
    return true;
}

void source(char **args)
{
    if (!args[1])
    {
        warn("source: not enough arguments\n");
        update_last_status(2);
        return;
    }
    char *path = NULL;
    if (!strchr(args[1], '/'))
    {
        char *env = strdup(getenv("PATH"));
        while ((path = strsep(&env, ":")) != NULL)
        {
            char *path_file = xcalloc(strlen(path) + strlen(args[1]) + 2, sizeof(char));
            strcat(path_file, path);
            strcat(path_file, "/");
            strcat(path_file, args[1]);
            if (load_file(path_file, false))
            {
                update_last_status(0);
                return;
            }
        }
    }
    for (int i = 2; args[i]; i++)
        append_program_data(args[i]);
    if (load_file(args[1], true))
        update_last_status(0);
    else
    {
        warn("source: no such file or directory %s\n", args[1]);
        update_last_status(2);
    }
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
                dprintf(1, "%c", tab[index_tab].corresp);
                index_tab = 0;
                i++;
                break;
            }
            index_tab++;
        }
        if (index_tab == 9)
            dprintf(1, "%c", c[i]);
        i++;
    }
    return (0);
}

static void print_echo(char **args, bool e, bool n)
{
    if (e == false && n == false)
    {
        for (int i = 0; args[i]; i++)
        {
            dprintf(1, "%s", args[i]);
            if (args[i + 1])
                dprintf(1, "%c", ' ');
        }
        dprintf(1, "%c",'\n');
    }
    else if (n == true && e == false)
    {
        for (int i = 0; args[i]; i++)
        {
            dprintf(1, "%s", args[i]);
            fflush(stdout);
            //fprintf(stdout, "%s", args[i]);
            if (args[i + 1])
                dprintf(1, "%c", ' ');
        }
    }
    else if (e == true)
    {
        for (int i = 0; args[i]; i++)
        {
            print_without_sp(args[i]);
            if (args[i + 1])
                dprintf(1, "%c", ' ');
        }
        if (n == false)
            dprintf(1, "%c", '\n');
    }
}

void echo(char **args)
{
    bool n = false;
    bool e = false;
    if (!args[1])
    {
        dprintf(1,"%c",'\n');
        update_last_status(0);
        return;
    }
    int j = 1;
    while (args[j][0] == '-' && args[j])
    {
        int i = 0;
        for (i = 1; (args[j][i] == 'n'
            || args[j][i] == 'e') && args[j][i]; i++)
        {
            if (args[j][i] == 'n')
                n = true;
            else if (args[j][i] == 'e')
                e = true;
            else if (args[j][i] == 'E')
                e = false;
        }
        if (args[j][i] != 0)
        {
            print_echo(args + j, e, n);
            return;
        }
        if (!args[j + 1])
        {
            update_last_status(0);
            return;
        }
        j++;
    }
    //dprintf(2, "%d\n", n);
    //dprintf(2, "%d\n", e);
    print_echo(args + j, e, n);
    update_last_status(0);
}

void cd(char **args)
{
    int ret = 0;
    char *pwd = getenv("PWD");
    if (!args[1])
        ret = chdir(getenv("HOME"));
    else if (strcmp(args[1], "-") == 0)
    {
        dprintf(1, "%s\n", getenv("OLDPWD"));
        ret = chdir(getenv("OLDPWD"));
    }
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
    char *s;
    const char *delim = "=";
    if (!args[0])
    {
        for (char **env = environ; *env != NULL; env++)
        {
            s = strtok(*env, delim);
            if (s)
                dprintf(1, "declare -x %s", s);
            s = strtok(*env, delim);
            if (s)
                dprintf(1,"=\"%s\"\n", s);
        }
        update_last_status(0);
        return;
    }
    else if (args[0][0] ==  '-')
    {
        for (int i = 1; args[0][i] && (args[0][i] == 'p'
            || args[0][i] == 'n'); i++)
        {
            if (args[0][i] == 'p')
            {
                p = true;
            }
            if (args[0][i] == 'n')
                n = true;
        }
        if ((!p && !n) || ( p && n))
        {
            fprintf(stderr, "export: bad option\n");
            update_last_status(2);
            return;
        }
    }
    if (p)
    {
        if (args[1])
            dprintf(1, "declare -x %s=\"%s\"\n", args[1], getenv(args[1]));
        else
            for (char **env = environ; *env != NULL; env++)
            {
                s = strtok(*env, delim);
                if (s)
                    dprintf(1, "declare -x %s", s);
                s = strtok(*env, delim);
                if (s)
                    dprintf(1, "=\"%s\"\n", s);
            }
   }
    else if (n)
    {
        if (!args[1])
        {
            fprintf(stderr, "export: bad option\n");
            update_last_status(1);
            return;
        }
        for (int i = 1; args[i]; i++)
            unsetenv(args[i]);
    }
    else
    {
        char *str;
        for (int i = 0; args[i]; i++)
        {
            if (getenv(args[i]))
            {
                str = xmalloc(strlen(args[i]) + strlen(getenv(args[i]) + 3));
                sprintf(str, "%s=\"%s\"", args[i],
                    getenv(args[i]));
            }
            else if (var_exists(var_storage, args[i]))
            {
                str = xmalloc(strlen(args[i]) +
                strlen(get_value(var_storage, args[i]) + 3));
                sprintf(str, "%s=\"%s\"", args[i],
                    get_value(var_storage, args[i]));
            }
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
    if (args)
    {
        if (strcmp(args[0], "continue") == 0)
        {
            dprintf(1, "%s", "bash: continue: only meaningful in a `for', ");
            dprintf(1,"%s","`while', or `until' loop\n");
        }
        update_last_status(0);
    }
}
void func_break(char **args)
{
    if (args)
    {
        if (strcmp(args[0], "break") == 0)
        {
            dprintf(1, "%s","bash: break: only meaningful in a `for', ");
            dprintf(1, "%s","`while', or `until' loop\n");
        }
        update_last_status(0);
    }
}
