#include "expansion.h"
#include "../utils/bracket_counter.h"

char *substitute(char *word)
{
    char *substituted = NULL;
    substituted = perform_tilde_expansion(word);
    substituted = perform_var_expansion(substituted);
    // printf("exp: %s\n", substituted);
    substituted = perform_command_substitution(substituted);
    // printf("exp: %s$\n", substituted);
    // if (!check_closing_symbols(substituted))
    //     return NULL;
    return substituted;
}
