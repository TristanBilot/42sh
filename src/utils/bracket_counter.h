/**
** \file bracket_counter.h
** \author Team 
** \brief 
** \version 0.1
** \date 2020-05-16
** 
** @copyright Copyright (c) 2020
** 
*/

#ifndef BRACKET_COUNTER_H
#define BRACKET_COUNTER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

enum countable
{
    COUNT_BRACK,
    COUNT_PAREN,
    COUNT_SING_QUOTE,
    COUNT_DOUB_QUOTE
};

/**
** \brief 
** 
** \param s 
** \param i 
** \param countable 
** \return int 
*/
int count_closed_occurences(char *s, size_t i, enum countable countable);
/**
** \brief 
** 
** \param s 
** \return true 
** \return false 
*/
bool check_closing_symbols(char *s);
/**
** \brief 
** 
** \param splitted 
** \param i 
** \return true 
** \return false 
*/
bool check_closing_symbols_from_splitted(char **splitted, int i);
/**
** \brief Get the closing parent index
** 
** \param word 
** \param i 
** \return int 
*/
int get_closing_parent_index(char *word, size_t i);

#endif /* BRACKET_COUNTER_H */
