#ifndef MAIN_H
#define MAIN_H

# define USAGE "alors il faut mettre le usage là mais c casse couille"

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

#endif /* ! MAIN_H */
