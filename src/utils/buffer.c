#include <stdlib.h>
#include <string.h>
#include "utils/buffer.h"

struct buffer *new_buffer()
{
    struct buffer *buffer = calloc(sizeof(struct buffer), 1);
    buffer->buf = calloc(BUFFER_SIZE, 1);
    buffer->index = 0;
    return buffer;
}

void append_buffer(struct buffer *buffer, char c)
{
    if (buffer->index >= BUFFER_SIZE)
        return;
    buffer->buf[buffer->index++] = c;
}

int buffer_len(struct buffer *buffer)
{
    return strlen(buffer->buf);
}

void free_buffer(struct buffer *buffer)
{
    free(buffer->buf);
    free(buffer);
}

void flush(struct buffer *buffer)
{
    buffer->index = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer->buf[i] = '\0';
}
