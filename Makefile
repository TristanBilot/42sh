NONE=

CC?= gcc
CPPFLAGS= \
          -D_GNU_SOURCE \
          -Isrc         \
          ${NONE}
CFLAGS= \
        -g \
        -Wextra \
        -Wall \
        -pedantic \
        -std=c99 \
        ${NONE}

OBJS= \
      src/parser/parser.o     \
      src/ast/ast.o     \
      src/ast/free.o     \
      src/lexer/token.o       \
      src/lexer/lexer.o \
      src/lexer/lex_evaluation.o \
      src/utils/xalloc.o \
      src/utils/string.o \
      src/utils/buffer.o \
      src/utils/parser_utils.o \
      src/print/ast_print_dot.o \
      src/exec/exec.o \
      ${NONE}

TEST_OBJS = \
      tests/tests_parser.o
      #tests/tests_lexer.o
      # tests/tests_ast.o \
      
BINS = \
      42sh        \
      token_printer \
      ast_print \
      exec

BINS_OBJS = \
            src/main.o              \
            src/eval/token_printer.o \
            src/eval/ast_print.o \
            src/eval/rpn_print.o \
            src/exec/exec.o \
            #${NONE}

TEST_BINS = \
            run_test_lexer \
            run_test_ast \
            run_test_parser

all: ${BINS}

debug: CFLAGS+= -g -fsanitize=address
debug: LDFLAGS+= -fsanitize=address
debug: all

tests: LDFLAGS+= -lcriterion
tests: LDFLAGS+= #-fsanitize=address
tests: run_test_lexer run_test_ast run_test_parser

42sh: src/main.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

token_printer: src/eval/token_printer.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ast_print: src/eval/ast_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

exec: src/exec/ast_exec.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

rpn_print: src/eval/rpn_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

run_test_lexer: tests/tests_lexer.o ${OBJS} ${TEST_OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

run_test_ast: tests/tests_ast.o ${OBJS} ${TEST_OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

run_test_parser: tests/tests_parser.o ${OBJS} ${TEST_OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

clean:
	${RM} ${OBJS} ${BINS} ${BINS_OBJS} ${TEST_BINS}

.PHONY: all
