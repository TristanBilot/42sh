#include <fcntl.h>
#include <stdio.h>
#include "../print/ast_print_dot.h"

FILE *new_dot(void)
{
    remove(DEFAULT_DOT_FILE_NAME);
    FILE *f = fopen(DEFAULT_DOT_FILE_NAME, "a");
    append_to_dot(f, "digraph AST {", true);
    return f;
}

bool append_to_dot(FILE *dot_file, const char *str, bool is_new_line)
{
    if (!dot_file)
        return false;
    char *format = is_new_line ? "%s\n" : "%s";
    int nb_bytes = fprintf(dot_file, format, str);
    return nb_bytes != 0 ? true : false;
}

bool close_dot(FILE *dot_file)
{
    if (!dot_file)
        return false;
    append_to_dot(dot_file, "}", true);
    fclose(dot_file);
    return true;
}

void convert_dot_to_png(void)
{
    char *dot_program = "dot";
    pid_t pid = 0;
    int status = 0;
    if ((pid = fork()) == 0)
    {
        int dev_null = open("/dev/null",0);
        dup2(dev_null, STDERR_FILENO);
        execlp(dot_program, dot_program, " -Tpng", DEFAULT_DOT_FILE_NAME,
        "-o", DEFAULT_PNG_FILE_NAME, NULL);
    }
    else
        waitpid(pid, &status, 0);
}
