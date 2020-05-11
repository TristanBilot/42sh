#include "expansion.h"

char *substitute(char *word)
{
    char *substituted = NULL;
    substituted = perform_tilde_expansion(word);
    substituted = perform_var_expansion(substituted);
    // substituted = perform_command_substitution(substituted);
    return substituted;
}
