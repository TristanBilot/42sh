#ifndef BUFFER_H
#define BUFFER_H

#include "../lexer/lexer.h"
#define BUFFER_SIZE 256

struct buffer {
    char *buf;
    int index;
};
/**
**\brief Create buffer
**
**\return struct buffer* 
*/
struct buffer *new_buffer();
/**
**\brief Append characters to the buffer
**
**\param buffer 
**\param c 
*/
void append_buffer(struct buffer *buffer, char c);
/**
**\brief Append string to the buffer
**
**\param buffer 
**\param str 
*/
void append_string_to_buffer(struct buffer *buffer, char *str);
/**
**\brief Free the buffer
**
**\param buffer 
*/
void free_buffer(struct buffer *buffer);
/**
**\brief Give the len of the buffer
**
**\param buffer 
**\return size_t 
*/
size_t buffer_len(struct buffer *buffer);
/**
**\brief Append word to buffer
**
**\param lexer 
**\param buffer 
*/
void append_word_if_needed(struct lexer *lexer, struct buffer *buffer);

/**
** \brief Empty a string buffer.
**
** \param buffer the string to be clear.
** \param size the length of the buffer.
*/
void flush(struct buffer *buffer);

#endif /* ! BUFFER_H */
