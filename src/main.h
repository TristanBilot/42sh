#ifndef MAIN_H
#define MAIN_H

# define USAGE "Usage : ./42sh [GNU long option] [option] [file]\n"

# define START_COLOR    "\033"
# define CYAN           "36m"
# define BLINK          "\033[5m"
# define END_COLOR      "\033[0m"

#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./parser/parser.h"
#include "./lexer/lexer.h"
#include "./utils/xalloc.h"
#include "./exec/exec.h"
#include "./utils/string_utils.h"
#include "./eval/ast_print.h"
#include "./var_storage/var_storage.h"
#include "./expansion/expansion.h"
#include "./garbage_collector/garbage_collector.h"
#include "./history/history.h"
#include "./utils/buffer.h"

struct option_sh
{
    bool norc_flag;
    bool print_ast_flag;
    char *cmd;
};

struct option_sh *option;

void init_42sh_with_history(struct option_sh *option);
void init_42sh_without_history(struct option_sh *option);

void print_usage(void);
int print_prompt(void);
void delete_last_character(void);
int file_exists(const char *filename);
void sighandler(int signum);
bool sould_use_history(void);
int getch2(void);


struct option_sh *init_option_sh(void);

#endif /* ! MAIN_H */
