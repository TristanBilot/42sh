/**
** \file xalloc.h
** \author Team
** \brief Special allocation functions
** \version 0.1
** \date 2020-05-03
**
** \copyright Copyright (c) 2020
**
*/
#ifndef XALLOC_H
#define XALLOC_H


#include <stddef.h>

/**
** \brief Safe malloc wrapper
** \param size the size to allocate
** \return a pointer to the allocated memory
*/
void *xmalloc(size_t size);

/**
** \brief Safe realloc wrapper
** \param ptr the pointer to reallocate
** \param size the new size to allocate
** \return a pointer to the allocated memory
*/
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t nmb, size_t size);

#endif /* ! XALLOC_H */
