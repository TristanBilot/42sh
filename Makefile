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
      src/ast/ast.o \
      src/lexer/lexer.o \
      src/lexer/token.o \
      src/parser/parser.o \
      src/utils/xalloc.o \
      ${NONE}

BINS= \
      token_printer \
      ast_print \
      rpn_print \
      ${NONE}

BINS_OBJS = \
            src/eval/token_printer.o \
            src/eval/ast_print.o \
            src/eval/rpn_print.o \
            ${NONE}


all: ${BINS}

debug: CFLAGS+= -g -fsanitize=address
debug: LDFLAGS+= -fsanitize=address
debug: all

token_printer: src/eval/token_printer.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ast_print: src/eval/ast_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

rpn_print: src/eval/rpn_print.o ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

clean:
	${RM} ${OBJS} ${BINS} ${BINS_OBJS}

.PHONY: all
