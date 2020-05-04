#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "../utils/xalloc.h"
#include "../garbage_collector/garbage_collector.h"

void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage(ret);
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    // printf("ATTENTION A FREE LE REALLOC");
    void *ret = realloc(ptr, size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage(ret);
    return ret;
}

void *xcalloc(size_t nmb, size_t size)
{
    void *ret = calloc(nmb, size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage(ret);
    return ret;
}

void *ymalloc(size_t size)
{
    void *ret = malloc(size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage_variable(ret);
    return ret;
}
void *yrealloc(void *ptr, size_t size)
{
    // printf("ATTENTION A FREE LE REALLOC");
    void *ret = realloc(ptr, size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage_variable(ret);
    return ret;
}

void *ycalloc(size_t nmb, size_t size)
{
    void *ret = calloc(nmb, size);
    if (size && !ret)
        err(1, "%s", strerror(errno));
    append_to_garbage_variable(ret);
    return ret;
}
