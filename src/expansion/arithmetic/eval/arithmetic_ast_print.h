#ifndef ARITHMETIC_AST_PRINT_H
#define ARITHMETIC_AST_PRINT_H

#include "../ast/arithmetic_ast.h"
#include "../lexer/arithmetic_lexer.h"

#include <stdbool.h>

void print_arithmetic_ast(struct arithmetic_ast *ast);

#endif /* ! ARITHMETIC_AST_PRINT_H */
