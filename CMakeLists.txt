cmake_minimum_required(VERSION 3.0)
enable_testing()

# set the project name
project(42sh)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "-g -Wall -Wextra -std=c99 -pedantic -fsanitize=address") #-Werror 

include_directories(src src/var_storage src/eval src/parser src/ast src/lexer src/utils src/exec src/print /usr/local/include/criterion/)

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
	src/utils/index_utils.c
	src/utils/my_itoa.c
	src/utils/main_utils.c
	src/eval/ast_print.c
	src/print/ast_print_dot.c
	src/exec/exec_for.c
	src/exec/exec.c
	src/var_storage/var_storage.c
	src/history/history.c
	src/history/auto_completion.c
	src/history/handler.c
	src/expansion/var_expansion.c
	src/expansion/tilde_expansion.c
	src/expansion/command_substitution.c
	src/expansion/expansion.c
	src/expansion/my_popen.c
	src/exec/commands.c
	src/garbage_collector/garbage_collector.c)


add_executable(tests_lexer tests/tests_lexer.c ${SOURCE_FILES})
add_executable(tests_parser tests/tests_parser.c ${SOURCE_FILES})
add_executable(tests_storage tests/tests_storage.c ${SOURCE_FILES})
add_executable(tests_history tests/tests_history.c ${SOURCE_FILES})
add_executable(ast_print src/print/ast_print_main.c ${SOURCE_FILES})
add_executable(token_printer src/eval/token_printer.c ${SOURCE_FILES})
add_executable(42sh src/main.c ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

find_package(Criterion REQUIRED)
target_link_libraries(tests_lexer PRIVATE ${CRITERION_LIBRARIES})
target_include_directories(tests_lexer PRIVATE ${CRITERION_INCLUDE_DIRS})

target_link_libraries(tests_parser PRIVATE ${CRITERION_LIBRARIES})
target_include_directories(tests_parser PRIVATE ${CRITERION_INCLUDE_DIRS})

target_link_libraries(tests_storage PRIVATE ${CRITERION_LIBRARIES})
target_include_directories(tests_storage PRIVATE ${CRITERION_INCLUDE_DIRS})

target_link_libraries(tests_history PRIVATE ${CRITERION_LIBRARIES})
target_include_directories(tests_history PRIVATE ${CRITERION_INCLUDE_DIRS})

add_custom_target(doc
	${PROJECT_SOURCE_DIR}/documentation.sh
	WORKING_DIRECTORY ../)

add_custom_target(check
	python3 ${PROJECT_SOURCE_DIR}/test_suite.py -t 2 build/42sh
	WORKING_DIRECTORY ../)