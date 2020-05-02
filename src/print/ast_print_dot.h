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

#define AST_STYLE_LOGIC \
  "style=filled color=\"1.0 .3 .7\" fontname=\"Helvetica\" fontsize=12 "

# define AST_STYLE_FUNCTION \
  "style=filled,dotted "    \
  "fontname=\"Helvetica\" fontsize=9"
/**
**\brief create new dote file
**
**\return FILE* 
*/
FILE *new_dot(void);
/**
**\brief append line to the dot file
**
**\param dot_file 
**\param str 
**\param is_new_line 
**\return true 
**\return false 
*/
bool append_to_dot(FILE *dot_file, const char *str, bool is_new_line);
/**
**\brief close dot file
**
**\param dot_file 
**\return true 
**\return false 
*/
bool close_dot(FILE *dot_file);
/**
**\brief convert file dot to png
**
*/
void convert_dot_to_png(void);
/**
**\brief create string
**
**\param ptr 
**\return char* 
*/
char *str(void *ptr);
/**
**\brief concatenate string
**
**\param arr 
**\return char* 
*/
char *concat(char *arr[]);

#endif /* ! AST_PRINT_DOT_H */
