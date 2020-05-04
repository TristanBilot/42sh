#include <stdio.h>
#include <stdlib.h>
#include "garbage_collector.h"

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
/******************************************************************* GARBAGE VARIABLE ********************************************************************************************/
 

void new_garbage_collector_variable(void)
{
    garbage_collector_variable = calloc(1, sizeof(struct garbage_collector_variable));
    garbage_collector_variable->first = NULL;
    garbage_collector_variable->next = NULL;
    garbage_collector_variable->last = NULL;
}

void append_to_garbage_variable(void *addr)
{
    if (!addr)
        return;

    struct garbage_variable *new = calloc(1, sizeof(struct garbage_variable));
    new->addr = addr;
    if (!garbage_collector_variable->first)
    {
        garbage_collector_variable->next = new;
        garbage_collector_variable->last = new;
        garbage_collector_variable->first = new;
        garbage_collector_variable->next->next = NULL;
        return;
    }
    garbage_collector_variable->last->next = new;
    garbage_collector_variable->last = new;
}

void free_garbage_collector_variable(void)
{
    if (!garbage_collector_variable)
        return;
    struct garbage_variable *index = garbage_collector_variable->first;
    struct garbage_variable *tmp = NULL;
    
    while (index)
    {
        tmp = index;
        index = index->next;
        free(tmp->addr);
        free(tmp);
        tmp = NULL;
    }
    garbage_collector_variable->first = NULL;
    garbage_collector_variable->next = NULL;
    garbage_collector_variable->last = NULL;
}

void print_garbage_collector_variable(void)
{
    struct garbage_variable *index = garbage_collector_variable->first;
    struct garbage_variable *tmp = NULL;
    int i = 0;
    while (index)
    {
        tmp = index;
        index = index->next;
        printf("addresse pointeur element %d : %p\n", i , tmp->addr);
        i++;
    }
}
/*struct size_minimum{
    int color;
};
struct chibrax
{
    struct chibrax *next_chibrax;
    struct size_minimum *min;
    int size_of_chibrax;
    int is_broken; //rupture du frein
    char *chibrax_name; //mr.popo
};
*/
// struct size_minimum *create_size_minimum(int color)
// {
//     struct size_minimum *new = malloc(sizeof(struct size_minimum));
//     new->color = color;
//     return new;
// }
// struct chibrax *create_chibrax(struct size_minimum *a, int size, int broken, char *name)
// {
//     struct chibrax *new = malloc(sizeof(struct chibrax));
//     new->next_chibrax = NULL;
//     new->min = a;
//     new->size_of_chibrax = size;
//     new->is_broken = broken;
//     new->chibrax_name = name;
//     return new;
// }
/*int main(void)
{
    struct size_minimum *size_minimum = create_size_minimum(1);
    struct size_minimum *size_minimum2 = create_size_minimum(1);
    struct size_minimum *size_minimum3 = create_size_minimum(1);
    struct size_minimum *size_minimum4 = create_size_minimum(1);
    struct chibrax *new = create_chibrax(size_minimum, 30, 0, "yolo");
    printf("pointeur du size_minimum : %p\n", size_minimum);
    printf("pointeur du chibrax : %p\n", new);
    new_garbage_collector();
    char *str = malloc(4 * sizeof(char));
    strcpy(str, "fsj");
    //printf("pointeur du string : %p\n", str);
    append_to_garbage(size_minimum);
    append_to_garbage(size_minimum2);
    append_to_garbage(size_minimum3);
    append_to_garbage(size_minimum4);
    append_to_garbage(new);
    append_to_garbage(str);
    printf("pointeur du garbage collector : %p\n", garbage_collector);
    print_garbage_collector();
    printf("pointeur garbage collector de l'élement : %p\n", garbage_collector->first->addr);
    free_garbage_collector();
    if (!garbage_collector->first)
        printf("Success clean\n");
    return 0;
}
*/