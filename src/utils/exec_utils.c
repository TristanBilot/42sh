#include <stdlib.h>
#include "../exec/exec.h"

struct list_stdin *append_list_stdin(struct tab_redirection *tab, char *value)
{
    if (!tab)
        return NULL;
    struct list_stdin *new = malloc(sizeof(struct list_stdin *));
    new->next = NULL;
    new->dest = value;
    if (!tab->stdin)
    {
        tab->stdin = new;
        return tab->stdin;
    }
    struct list_stdin *first = tab->stdin;
    while (first)
    {
        if (!first->next)
        {
            first->next = new;
            return tab->stdin;
        }
        first = first->next;
    }
    return tab->stdin;
}

struct list_stdout *append_list_stdout(struct tab_redirection *tab, char *value)
{
    if (!tab)
        return NULL;
    struct list_stdout *new = malloc(sizeof(struct list_stdout *));
    new->next = NULL;
    new->dest = value;
    if (!tab->stdout)
    {
        tab->stdout = new;
        return tab->stdout;
    }
    struct list_stdout *first = tab->stdout;
    while (first)
    {
        if (!first->next)
        {
            first->next = new;
            return tab->stdout;
        }
        first = first->next;
    }
    return tab->stdout;
}

struct list_stderr *append_list_stderr(struct tab_redirection *tab, char *value)
{
    if (!tab)
        return NULL;
    struct list_stderr *new = malloc(sizeof(struct list_stderr *));
    new->next = NULL;
    new->dest = value;
    if (!tab->stderr)
    {
        tab->stderr = new;
        return tab->stderr;
    }
    struct list_stderr *first = tab->stderr;
    while (first)
    {
        if (!first->next)
        {
            first->next = new;
            return tab->stderr;
        }
        first = first->next;
    }
    return tab->stderr;
}

void free_tab(struct tab_redirection *tab)
{
    if (tab)
    {
        while (tab->stdin)
        {
            struct list_stdin *in = tab->stdin;
            tab->stdin = tab->stdin->next;
            free(in);
        }
        while (tab->stdout)
        {
            struct list_stdout *out = tab->stdout;
            tab->stdout = tab->stdout->next;
            free(out);
        }
        while (tab->stderr)
        {
            struct list_stderr *err = tab->stderr;
            tab->stderr = tab->stderr->next;
            free(err);
        }
        free(tab);
    }
}