#include "utils/xalloc.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>


void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (size && !ret)
        err(1, "malloc failed");
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (size && !ret)
        err(1, "realloc failed");
    return ret;
}
