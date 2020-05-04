/**
** \file index_utils.h
** \author Team 
** \brief Index functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/
#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <stddef.h>

int is_separator(char c);
size_t get_next_index(const char *str, char c, size_t i);
size_t get_previous_index(const char *str, char c, size_t i);
size_t get_previous_separator_index(const char *str, size_t j);
size_t get_next_separator_index(const char *c, size_t j);
size_t get_next_close_curl_index(const char *str, size_t j);

#endif /* INDEX_UTILS_H */
