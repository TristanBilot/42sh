#ifndef ARITHMETIC_EXECUTION_H
#define ARITHMETIC_EXECUTION_H

#include "../ast/arithmetic_ast.h"
#include <stdbool.h>

double exec_arithmetic_ast(struct arithmetic_ast *ast, bool *error);

#endif /* ARITHMETIC_EXECUTION_H */
