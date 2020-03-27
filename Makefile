NONE=

CC?= gcc
CPPFLAGS= \
          -D_GNU_SOURCE \
          -Isrc \
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
      tests/tests_lexer.o

BINS= \
      token_printer \
      # ast_print \
      # rpn_print \
      # ${NONE}

BINS_OBJS = \
            src/eval/token_printer.o \
            ${NONE}
            # src/eval/ast_print.o \
            # src/eval/rpn_print.o \
            #${NONE}


all: ${BINS}

debug: CFLAGS+= -g -fsanitize=address
debug: LDFLAGS+= -fsanitize=address
debug: all

tests: LDFLAGS+= -lcriterion
tests: run_test

token_printer: src/eval/token_printer.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ast_print: src/eval/ast_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

rpn_print: src/eval/rpn_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

run_test: tests/tests_lexer.o ${OBJS} ${TEST_OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

clean:
	${RM} ${OBJS} ${BINS} ${BINS_OBJS}

.PHONY: all
