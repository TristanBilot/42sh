#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "../exec/redirection.h"
#include "../utils/string_utils.h"
#include "../utils/my_itoa.h"

#define STDOUT_FILENO 1
#define STDIN_FILENO 0

struct file_manager *init_file_manager(void)
{
    struct file_manager *file_manager = xcalloc(1, sizeof(struct file_manager));
    file_manager->fd_to_close = -1;
    file_manager->file = NULL;
    file_manager->save_in = dup(0);
    file_manager->save_in = dup(1);
    file_manager->save_err = dup(2);
    return file_manager;
}

struct tab_redirection init_tab_redirection(void)
{
    struct tab_redirection tab;
    tab.redirections = NULL;
    tab.size = 0;
    return tab;
}

struct tab_redirection append_tab_redirection(struct tab_redirection tab, struct node_redirection *e)
{
    struct std new;
    new.type = type_to_str(e->type);
    new.file = e->right;
    if (is(e->left, ""))
    {
        if (is(new.type, "TOK_LESS"))
            new.ionumber = 0;
        else
            new.ionumber = 1;
    }
    else
        new.ionumber = atoi(e->left);
    tab.redirections = xrealloc(tab.redirections, sizeof(tab.redirections) + sizeof(struct std));
    tab.redirections[tab.size++] = new;
    return tab;
}

bool manage_duplication(struct tab_redirection tab)
{
    for (int i = 0; i < tab.size; i++)
    {
        if (is(tab.redirections[i].type, "TOK_GREAT"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber);
        else if (is(tab.redirections[i].type, "TOK_LESS"))
            return dup_file(tab.redirections[i].file, "r", tab.redirections[i].ionumber);
        else if (is(tab.redirections[i].type, "TOK_DGREAT"))
            return dup_file(tab.redirections[i].file, "a+", tab.redirections[i].ionumber);
        else if (is(tab.redirections[i].type, "TOK_DLESS"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber); // à faire
        else if (is(tab.redirections[i].type, "TOK_DLESSDASH"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber); // à faire
        else if (is(tab.redirections[i].type, "TOK_LESSGREAT"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber); // à faire
        else if (is(tab.redirections[i].type, "TOK_LESSAND"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber); // à faire
        else if (is(tab.redirections[i].type, "TOK_GREATAND"))
            return dup2(tab.redirections[i].ionumber, atoi(tab.redirections[i].file)) < 0;
        else if (is(tab.redirections[i].type, "TOK_CLOBBER"))
            return dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber); // à faire
    }
    return false;
}

bool dup_file(char *file, char *flag, int io)
{
    FILE *f = fopen(file, flag);
    if (!f)
        return true;
    int out = fileno(f);
    if (out == -1)
    {
        return true;
    }
    file_manager->fd_to_close = out;
    file_manager->file = f;
    close(io);
    int fd = dup2(out, io);
    if (fd < 0)
    {
        close(out);
        return true;
    }
    // close(fd);
    close(out);
    file_manager->fd_to_close = -1;
    file_manager->file = NULL;
    fclose(f);
    return false;
}