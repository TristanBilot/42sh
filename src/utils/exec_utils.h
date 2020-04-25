#ifndef EXEC_UTILS_H
#define EXEC_UTILS_H

struct list_stdin
{
    char *dest;
    struct list_stdin *next;
};

struct list_stdout
{
    char *dest;
    struct list_stdout *next;
};

struct list_stderr
{
    char *dest;
    struct list_stderr *next;
};

struct tab_redirection
{
    struct list_stdin *stdin;
    struct list_stdout *stdout;
    struct list_stderr *stderr;
};

struct list_stdin *append_list_stdin(struct tab_redirection *tab, char *value);
struct list_stdout *append_list_stdout(struct tab_redirection *tab, char *value);
struct list_stderr *append_list_stderr(struct tab_redirection *tab, char *value);
void free_tab(struct tab_redirection *tab);

#endif /* EXEC_UTILS_H */