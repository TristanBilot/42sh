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
        if (is(new.type, "TOK_LESS") || is(new.type, "TOK_LESSAND")|| is(new.type, "TOK_LESSGREAT"))
            new.ionumber = 0;
        else
            new.ionumber = 1;
    }
    else
        new.ionumber = atoi(e->left);
    if (tab.size < TAB_REDI_SIZE)
    {
        tab.redirections[tab.size] = new;
        tab.size += 1;
    }
    return tab;
}

bool manage_duplication(struct tab_redirection tab)
{
    for (int i = 0; i < tab.size; i++)
    {
        if (is(tab.redirections[i].type, "TOK_GREAT"))  // ne doit pas fonctionner quand noclobber est set
        {
            if (dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_LESS"))
        {
            if (dup_file(tab.redirections[i].file, "r", tab.redirections[i].ionumber))
                return true;
        }  
        else if (is(tab.redirections[i].type, "TOK_DGREAT"))
        {
            if (dup_file(tab.redirections[i].file, "a", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_DLESS")) // HEREDOC à faire
        {
            if (dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_DLESSDASH")) // HEREDOC à faire
        {
            if (dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_LESSGREAT"))
        {
            if (dup_file(tab.redirections[i].file, "r+", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_LESSAND"))
        {
            if (is(tab.redirections[i].file, "-"))
                close(tab.redirections[i].ionumber);
            else if (tab.redirections[i].file[0] && tab.redirections[i].file[1] && tab.redirections[i].file[1] == '-')
            {
                dup_fd(tab.redirections[i].file[0] - '0', "r", tab.redirections[i].ionumber);
                close(tab.redirections[i].file[0] - '0');
            }
            else if (atoi(tab.redirections[i].file) == 0 && !is(tab.redirections[i].file, "0"))
                return true;
            else if (dup_fd(atoi(tab.redirections[i].file), "r", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_GREATAND"))
        {
            if (is(tab.redirections[i].file, "-"))
                close(tab.redirections[i].ionumber);
            else if (tab.redirections[i].file[0] && tab.redirections[i].file[1] && tab.redirections[i].file[1] == '-')
            {
                dup_fd(tab.redirections[i].file[0] - '0', "a", tab.redirections[i].ionumber);
                close(tab.redirections[i].file[0] - '0');
            }
            else if (atoi(tab.redirections[i].file) == 0 && !is(tab.redirections[i].file, "0"))
            {
                if (tab.redirections[i].ionumber != 1 || dup_file(tab.redirections[i].file, "w+", 12))
                    return true;
            }
            else if (dup_fd(atoi(tab.redirections[i].file), "a", tab.redirections[i].ionumber))
                return true;
        }
        else if (is(tab.redirections[i].type, "TOK_CLOBBER"))
        {
            if (dup_file(tab.redirections[i].file, "w+", tab.redirections[i].ionumber))
                return true;
        }
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
    if (io == 12)
    {
        close(1);
        close(2);
        int fd = dup2(out, 1);
        if (fd < 0)
        {
            close(out);
            return true;
        }
        fd = dup2(out, 2);
        if (fd < 0)
        {
            close(out);
            return true;
        }
        close(out);
        file_manager->fd_to_close = -1;
        file_manager->file = NULL;
        fclose(f);
        return false;
    }
    close(io);
    int fd = dup2(out, io);
    if (fd < 0)
    {
        close(out);
        return true;
    }
    close(out);
    file_manager->fd_to_close = -1;
    file_manager->file = NULL;
    fclose(f);
    return false;
}

bool dup_fd(int file, char *flag, int io)
{
    FILE *f = fdopen(file, flag);
    if (!f)
        return true;
    int out = fileno(f);
    if (out == -1)
        return true;
    close(io);
    int fd = dup2(out, io);
    if (fd < 0)
    {
        close(out);
        return true;
    }
    // Ne pas close
    return false;
}