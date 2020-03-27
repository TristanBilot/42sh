#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 256

struct buffer {
    char *buf;
    int index;
};

struct buffer *new_buffer();
void append_buffer(struct buffer *buffer, char c);
void free_buffer(struct buffer *buffer);
int buffer_len(struct buffer *buffer);

/**
** \brief Empty a string buffer.
**
** \param buffer the string to be clear.
** \param size the length of the buffer.
*/
void flush(struct buffer *buffer);

#endif /* ! BUFFER_H */