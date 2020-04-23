cmake_minimum_required(VERSION 3.0)

# set the project name
project(42sh)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_C_FLAGS "-g -Wall -Wextra -std=c99 -pedantic") #-Werror 

set(SOURCE_FILES
	src/parser/parser.c
	src/ast/ast.c
	src/ast/free.c
	src/lexer/token.c
	src/lexer/lexer.c
	src/lexer/lex_evaluation.c
	src/utils/xalloc.c
	src/utils/string_utils.c
	src/utils/buffer.c
	src/utils/parser_utils.c
	src/print/ast_print_dot.c
	src/exec/exec.c
    src/main.c )

include_directories(src src/parser src/ast src/lexer src/utils src/exec src/print)

add_executable(42sh ${SOURCE_FILES})