#ifndef BUFFER_H
#define BUFFER_H

#include "../lexer/lexer.h"
#define BUFFER_SIZE 256

struct buffer {
    char *buf;
    int index;
};

struct buffer *new_buffer();
void append_buffer(struct buffer *buffer, char c);
void append_string_to_buffer(struct buffer *buffer, char *str);
void free_buffer(struct buffer *buffer);
size_t buffer_len(struct buffer *buffer);
void append_word_if_needed(struct lexer *lexer, struct buffer *buffer);

/**
** \brief Empty a string buffer.
**
** \param buffer the string to be clear.
** \param size the length of the buffer.
*/
void flush(struct buffer *buffer);

#endif /* ! BUFFER_H */
