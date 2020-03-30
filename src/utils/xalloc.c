#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils/xalloc.h"

void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    return ret;
}
