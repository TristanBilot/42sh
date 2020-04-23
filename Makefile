# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.16.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.16.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tristanbilot/Documents/dev/epita/42sh

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tristanbilot/Documents/dev/epita/42sh

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.16.5/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.16.5/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/tristanbilot/Documents/dev/epita/42sh/CMakeFiles /Users/tristanbilot/Documents/dev/epita/42sh/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/tristanbilot/Documents/dev/epita/42sh/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named 42sh

# Build rule for target.
42sh: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 42sh
.PHONY : 42sh

# fast build rule for target.
42sh/fast:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/build
.PHONY : 42sh/fast

src/ast/ast.o: src/ast/ast.c.o

.PHONY : src/ast/ast.o

# target to build an object file
src/ast/ast.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/ast.c.o
.PHONY : src/ast/ast.c.o

src/ast/ast.i: src/ast/ast.c.i

.PHONY : src/ast/ast.i

# target to preprocess a source file
src/ast/ast.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/ast.c.i
.PHONY : src/ast/ast.c.i

src/ast/ast.s: src/ast/ast.c.s

.PHONY : src/ast/ast.s

# target to generate assembly for a file
src/ast/ast.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/ast.c.s
.PHONY : src/ast/ast.c.s

src/ast/free.o: src/ast/free.c.o

.PHONY : src/ast/free.o

# target to build an object file
src/ast/free.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/free.c.o
.PHONY : src/ast/free.c.o

src/ast/free.i: src/ast/free.c.i

.PHONY : src/ast/free.i

# target to preprocess a source file
src/ast/free.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/free.c.i
.PHONY : src/ast/free.c.i

src/ast/free.s: src/ast/free.c.s

.PHONY : src/ast/free.s

# target to generate assembly for a file
src/ast/free.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast/free.c.s
.PHONY : src/ast/free.c.s

src/exec/exec.o: src/exec/exec.c.o

.PHONY : src/exec/exec.o

# target to build an object file
src/exec/exec.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/exec/exec.c.o
.PHONY : src/exec/exec.c.o

src/exec/exec.i: src/exec/exec.c.i

.PHONY : src/exec/exec.i

# target to preprocess a source file
src/exec/exec.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/exec/exec.c.i
.PHONY : src/exec/exec.c.i

src/exec/exec.s: src/exec/exec.c.s

.PHONY : src/exec/exec.s

# target to generate assembly for a file
src/exec/exec.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/exec/exec.c.s
.PHONY : src/exec/exec.c.s

src/lexer/lex_evaluation.o: src/lexer/lex_evaluation.c.o

.PHONY : src/lexer/lex_evaluation.o

# target to build an object file
src/lexer/lex_evaluation.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lex_evaluation.c.o
.PHONY : src/lexer/lex_evaluation.c.o

src/lexer/lex_evaluation.i: src/lexer/lex_evaluation.c.i

.PHONY : src/lexer/lex_evaluation.i

# target to preprocess a source file
src/lexer/lex_evaluation.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lex_evaluation.c.i
.PHONY : src/lexer/lex_evaluation.c.i

src/lexer/lex_evaluation.s: src/lexer/lex_evaluation.c.s

.PHONY : src/lexer/lex_evaluation.s

# target to generate assembly for a file
src/lexer/lex_evaluation.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lex_evaluation.c.s
.PHONY : src/lexer/lex_evaluation.c.s

src/lexer/lexer.o: src/lexer/lexer.c.o

.PHONY : src/lexer/lexer.o

# target to build an object file
src/lexer/lexer.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lexer.c.o
.PHONY : src/lexer/lexer.c.o

src/lexer/lexer.i: src/lexer/lexer.c.i

.PHONY : src/lexer/lexer.i

# target to preprocess a source file
src/lexer/lexer.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lexer.c.i
.PHONY : src/lexer/lexer.c.i

src/lexer/lexer.s: src/lexer/lexer.c.s

.PHONY : src/lexer/lexer.s

# target to generate assembly for a file
src/lexer/lexer.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/lexer.c.s
.PHONY : src/lexer/lexer.c.s

src/lexer/token.o: src/lexer/token.c.o

.PHONY : src/lexer/token.o

# target to build an object file
src/lexer/token.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/token.c.o
.PHONY : src/lexer/token.c.o

src/lexer/token.i: src/lexer/token.c.i

.PHONY : src/lexer/token.i

# target to preprocess a source file
src/lexer/token.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/token.c.i
.PHONY : src/lexer/token.c.i

src/lexer/token.s: src/lexer/token.c.s

.PHONY : src/lexer/token.s

# target to generate assembly for a file
src/lexer/token.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/lexer/token.c.s
.PHONY : src/lexer/token.c.s

src/main.o: src/main.c.o

.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/main.c.s
.PHONY : src/main.c.s

src/parser/parser.o: src/parser/parser.c.o

.PHONY : src/parser/parser.o

# target to build an object file
src/parser/parser.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/parser/parser.c.o
.PHONY : src/parser/parser.c.o

src/parser/parser.i: src/parser/parser.c.i

.PHONY : src/parser/parser.i

# target to preprocess a source file
src/parser/parser.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/parser/parser.c.i
.PHONY : src/parser/parser.c.i

src/parser/parser.s: src/parser/parser.c.s

.PHONY : src/parser/parser.s

# target to generate assembly for a file
src/parser/parser.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/parser/parser.c.s
.PHONY : src/parser/parser.c.s

src/print/ast_print_dot.o: src/print/ast_print_dot.c.o

.PHONY : src/print/ast_print_dot.o

# target to build an object file
src/print/ast_print_dot.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/print/ast_print_dot.c.o
.PHONY : src/print/ast_print_dot.c.o

src/print/ast_print_dot.i: src/print/ast_print_dot.c.i

.PHONY : src/print/ast_print_dot.i

# target to preprocess a source file
src/print/ast_print_dot.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/print/ast_print_dot.c.i
.PHONY : src/print/ast_print_dot.c.i

src/print/ast_print_dot.s: src/print/ast_print_dot.c.s

.PHONY : src/print/ast_print_dot.s

# target to generate assembly for a file
src/print/ast_print_dot.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/print/ast_print_dot.c.s
.PHONY : src/print/ast_print_dot.c.s

src/utils/buffer.o: src/utils/buffer.c.o

.PHONY : src/utils/buffer.o

# target to build an object file
src/utils/buffer.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/buffer.c.o
.PHONY : src/utils/buffer.c.o

src/utils/buffer.i: src/utils/buffer.c.i

.PHONY : src/utils/buffer.i

# target to preprocess a source file
src/utils/buffer.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/buffer.c.i
.PHONY : src/utils/buffer.c.i

src/utils/buffer.s: src/utils/buffer.c.s

.PHONY : src/utils/buffer.s

# target to generate assembly for a file
src/utils/buffer.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/buffer.c.s
.PHONY : src/utils/buffer.c.s

src/utils/parser_utils.o: src/utils/parser_utils.c.o

.PHONY : src/utils/parser_utils.o

# target to build an object file
src/utils/parser_utils.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/parser_utils.c.o
.PHONY : src/utils/parser_utils.c.o

src/utils/parser_utils.i: src/utils/parser_utils.c.i

.PHONY : src/utils/parser_utils.i

# target to preprocess a source file
src/utils/parser_utils.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/parser_utils.c.i
.PHONY : src/utils/parser_utils.c.i

src/utils/parser_utils.s: src/utils/parser_utils.c.s

.PHONY : src/utils/parser_utils.s

# target to generate assembly for a file
src/utils/parser_utils.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/parser_utils.c.s
.PHONY : src/utils/parser_utils.c.s

src/utils/string.o: src/utils/string.c.o

.PHONY : src/utils/string.o

# target to build an object file
src/utils/string.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/string.c.o
.PHONY : src/utils/string.c.o

src/utils/string.i: src/utils/string.c.i

.PHONY : src/utils/string.i

# target to preprocess a source file
src/utils/string.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/string.c.i
.PHONY : src/utils/string.c.i

src/utils/string.s: src/utils/string.c.s

.PHONY : src/utils/string.s

# target to generate assembly for a file
src/utils/string.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/string.c.s
.PHONY : src/utils/string.c.s

src/utils/xalloc.o: src/utils/xalloc.c.o

.PHONY : src/utils/xalloc.o

# target to build an object file
src/utils/xalloc.c.o:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/xalloc.c.o
.PHONY : src/utils/xalloc.c.o

src/utils/xalloc.i: src/utils/xalloc.c.i

.PHONY : src/utils/xalloc.i

# target to preprocess a source file
src/utils/xalloc.c.i:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/xalloc.c.i
.PHONY : src/utils/xalloc.c.i

src/utils/xalloc.s: src/utils/xalloc.c.s

.PHONY : src/utils/xalloc.s

# target to generate assembly for a file
src/utils/xalloc.c.s:
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/utils/xalloc.c.s
.PHONY : src/utils/xalloc.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... 42sh"
	@echo "... src/ast/ast.o"
	@echo "... src/ast/ast.i"
	@echo "... src/ast/ast.s"
	@echo "... src/ast/free.o"
	@echo "... src/ast/free.i"
	@echo "... src/ast/free.s"
	@echo "... src/exec/exec.o"
	@echo "... src/exec/exec.i"
	@echo "... src/exec/exec.s"
	@echo "... src/lexer/lex_evaluation.o"
	@echo "... src/lexer/lex_evaluation.i"
	@echo "... src/lexer/lex_evaluation.s"
	@echo "... src/lexer/lexer.o"
	@echo "... src/lexer/lexer.i"
	@echo "... src/lexer/lexer.s"
	@echo "... src/lexer/token.o"
	@echo "... src/lexer/token.i"
	@echo "... src/lexer/token.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/parser/parser.o"
	@echo "... src/parser/parser.i"
	@echo "... src/parser/parser.s"
	@echo "... src/print/ast_print_dot.o"
	@echo "... src/print/ast_print_dot.i"
	@echo "... src/print/ast_print_dot.s"
	@echo "... src/utils/buffer.o"
	@echo "... src/utils/buffer.i"
	@echo "... src/utils/buffer.s"
	@echo "... src/utils/parser_utils.o"
	@echo "... src/utils/parser_utils.i"
	@echo "... src/utils/parser_utils.s"
	@echo "... src/utils/string.o"
	@echo "... src/utils/string.i"
	@echo "... src/utils/string.s"
	@echo "... src/utils/xalloc.o"
	@echo "... src/utils/xalloc.i"
	@echo "... src/utils/xalloc.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

