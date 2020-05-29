/**
** \file garbage_collector.h
** \author Team
** \brief Execution functions
** \version 0.1
** \date 2020-05-03
**
** \copyright Copyright (c) 2020
**
*/

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

struct garbage_element
{
    struct garbage_element *next;
    void *addr;
};

struct garbage_collector
{
    struct garbage_element *first;
    struct garbage_element *next;
    struct garbage_element *last;
};

extern struct garbage_collector *garbage_collector;
/**
**\brief create the garbage collector
**
*/
void new_garbage_collector(void);
/**
**\brief append addr to list of elements
**
**\param addr
*/
void append_to_garbage(void *addr);
/**
**\brief free list of elements
**
*/
void free_garbage_collector();


#endif /* GARBAGE_COLLECTOR_H */
