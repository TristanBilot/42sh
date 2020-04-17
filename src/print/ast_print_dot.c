#include "ast_print_dot.h"

 /*
 digraph G {
     or -> ls;
     or -> pwd;
     itm [label=or];
     itm -> new;
 }
 dot -Tpng test.dot -o test.png
 */

FILE *new_dot(void)
{
    remove(DEFAULT_DOT_FILE_NAME);
    FILE *f = fopen(DEFAULT_DOT_FILE_NAME, "a");
    append_to_dot(f, "strict graph G {", true);
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
    char *args[] = { dot_program, " -Tpng", DEFAULT_DOT_FILE_NAME, "-o", DEFAULT_PNG_FILE_NAME, NULL };
    
    pid_t pid = 0;
    int status = 0;

    if ((pid = fork()) == 0)
        execvp(dot_program, args);
    else
        waitpid(pid, &status, 0);
}

char *str(void *ptr)
{
    char *s = "";
    sprintf(s, "%p", (void *)&ptr);
    return s;
}

char *concat(char *arr[])
{
    char *str = "";
    int i = 0;
    while (arr && arr[i])
    {
        strcat(str, arr[i++]);
    }
    return str;
}