#ifndef XALLOC_H
#define XALLOC_H

#include "../utils/attr.h"

#include <stddef.h>

/**
** \brief Safe malloc wrapper
** \param size the size to allocate
** \return a pointer to the allocated memory
*/
void *xmalloc(size_t size) __malloc;

/**
** \brief Safe realloc wrapper
** \param ptr the pointer to reallocate
** \param size the new size to allocate
** \return a pointer to the allocated memory
*/
void *xrealloc(void *ptr, size_t size);

void *xcalloc(size_t nmb, size_t size);


#endif /* ! XALLOC_H */
