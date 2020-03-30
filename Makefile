NONE=

CC?= gcc
CPPFLAGS= \
          -D_GNU_SOURCE \
          -Isrc         \
          ${NONE}
CFLAGS= \
        -g \
        -Werror \
        -Wextra \
        -Wall \
        -pedantic \
        -std=c99 \
        ${NONE}

OBJS= \
      src/lexer/token.o \
      src/lexer/lexer.o \
      src/lexer/lex_evaluation.o \
      src/utils/xalloc.o \
      src/utils/string.o \
      src/utils/buffer.o \
      ${NONE}

TEST_OBJS = \
      tests/tests_lexer.o \
      tests/tests_ast.o

BINS = token_printer

BINS_OBJS = \
            src/eval/token_printer.o \
            ${NONE}
            # src/eval/ast_print.o \
            # src/eval/rpn_print.o \
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
tests: run_test_lexer run_test_ast run_test_parser

token_printer: src/eval/token_printer.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ast_print: src/eval/ast_print.o ${OBJS}
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
