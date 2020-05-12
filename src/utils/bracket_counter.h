/**
** \file lexer.h
** \author Team 
** \brief Bracket counter functions
** \version 0.1
** \date 2020-05-12
** 
** \copyright Copyright (c) 2020
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

int count_closed_occurences(char *s, size_t i, enum countable countable);
bool check_closing_symbols(char *s);

#endif /* BRACKET_COUNTER_H */
