#ifndef AST_PRINT_DOT_H
#define AST_PRINT_DOT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define DEFAULT_DOT_FILE_NAME "ast.dot"
#define DEFAULT_PNG_FILE_NAME "ast.png"

FILE *new_dot(void);
bool append_to_dot(FILE *dot_file, const char *str, bool is_new_line);
bool close_dot(FILE *dot_file);
void convert_dot_to_png(void);
char *str(void *ptr);
char *concat(char *arr[]);

#endif /* ! AST_PRINT_DOT_H */
