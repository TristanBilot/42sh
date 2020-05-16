#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../utils/buffer.h"
#include "../utils/xalloc.h"

struct buffer *new_buffer(void)
{
    struct buffer *buffer = xcalloc(sizeof(struct buffer), 1);
    buffer->buf = xcalloc(BUFFER_SIZE, 1);
    buffer->index = 0;
    return buffer;
}

struct buffer *new_huge_buffer(void)
{
    struct buffer *buffer = xcalloc(sizeof(struct buffer), 1);
    buffer->buf = xcalloc(1, HUGE_BUFFER_SIZE);
    buffer->index = 0;
    return buffer;
}

void append_buffer(struct buffer *buffer, char c)
{
    if (buffer->index >= BUFFER_SIZE)
        return;
    buffer->buf[buffer->index++] = c;
}

void append_huge_buffer(struct buffer *buffer, char c)
{
    if (buffer->index >= HUGE_BUFFER_SIZE)
        return;
    buffer->buf[buffer->index++] = c;
}

void append_string_to_buffer(struct buffer *buffer, char *str)
{
    if (!str || !buffer)
        return;
    for (size_t j = 0; j < strlen(str); j++)
        if (!(j == strlen(str) - 1 && str[j] == '\n'))
            append_buffer(buffer, str[j]);
}

void append_string_to_huge_buffer(struct buffer *buffer, char *str)
{
    if (!str || !buffer)
        return;
    for (size_t j = 0; j < strlen(str); j++)
        if (!(j == strlen(str) - 1 && str[j] == '\n'))
            append_huge_buffer(buffer, str[j]);
}

size_t buffer_len(struct buffer *buffer)
{
    return strlen(buffer->buf);
}

void append_word_if_needed(struct lexer *lexer, struct buffer *buffer)
{
    if (buffer_len(buffer) > 0)
    {
        append(lexer, new_token_word(buffer->buf));
        flush(buffer);
    }
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
