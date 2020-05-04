#ifndef MAIN_H
#define MAIN_H

# define USAGE "Usage : ./42sh [GNU long option] [option] [file]\n"

# define START_COLOR    "\033"
# define CYAN           "36m"
# define BLINK          "\033[5m"
# define END_COLOR      "\033[0m"



struct option_sh
{
    bool norc_flag;
    bool print_ast_flag;
    char *cmd;
};

static struct option_sh *init_option_sh(void);
void init_42sh_process(struct option_sh *option);

#endif /* ! MAIN_H */
