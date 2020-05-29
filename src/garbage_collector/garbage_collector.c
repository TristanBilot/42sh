#include <stdio.h>
#include <stdlib.h>
#include "garbage_collector.h"

struct garbage_collector *garbage_collector;
void new_garbage_collector(void)
{
    garbage_collector = calloc(1, sizeof(struct garbage_collector));
    garbage_collector->first = NULL;
    garbage_collector->next = NULL;
    garbage_collector->last = NULL;
}

void append_to_garbage(void *addr)
{
    if (!addr)
        return;

    struct garbage_element *new = calloc(1, sizeof(struct garbage_element));
    new->addr = addr;
    if (!garbage_collector->first)
    {
        garbage_collector->next = new;
        garbage_collector->last = new;
        garbage_collector->first = new;
        garbage_collector->next->next = NULL;
        return;
    }
    garbage_collector->last->next = new;
    garbage_collector->last = new;
}

void free_garbage_collector(void)
{
    struct garbage_element *index = garbage_collector->first;
    struct garbage_element *tmp = NULL;
    while (index)
    {
        tmp = index;
        index = index->next;
        free(tmp->addr);
        free(tmp);
        tmp = NULL;
    }
    garbage_collector->first = NULL;
    garbage_collector->next = NULL;
    garbage_collector->last = NULL;
}

void print_garbage_collector(void)
{
    struct garbage_element *index = garbage_collector->first;
    struct garbage_element *tmp = NULL;
    int i = 0;
    while (index)
    {
        tmp = index;
        index = index->next;
        printf("addresse pointeur element %d : %p\n", i , tmp->addr);
        i++;
    }
}
