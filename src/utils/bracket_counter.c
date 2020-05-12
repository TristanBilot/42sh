#include "bracket_counter.h"

/*
* < 0 => error
* > 0 => waiting for input
* = 0 => perfect bro
*/
int count_closed_occurences(char *s, size_t i, enum countable countable)
{
    if (!s)
        return 0;
    char opening;
    char closing;
    switch (countable)
    {
    case COUNT_PAREN:
        opening = '(';
        closing = ')';
        break;
    case COUNT_BRACK:
        opening = '{';
        closing = '}';
        break;
    case COUNT_DOUB_QUOTE:
        opening = '"';
        closing = '"';
        break;
    case COUNT_SING_QUOTE:
        opening = '\'';
        closing = '\'';
        break;
    default:
        printf("Invalid countable @ count_closed_occurences()");
        return 0;
    }
    int count = 0;
    for (size_t j = i; j < strlen(s); j++)
    {
        if (s[j] == opening)
            count++;
        else if (s[j] == closing)
            count--;
    }
    return count;
}

/*
* true => word is valid
* false => word is not valid
*/
bool check_closing_symbols(char *s)
{
    int brack = count_closed_occurences(s, 0, COUNT_BRACK);
    int paren = count_closed_occurences(s, 0, COUNT_PAREN);
    int sin_q = count_closed_occurences(s, 0, COUNT_SING_QUOTE);
    int dou_q = count_closed_occurences(s, 0, COUNT_DOUB_QUOTE);
    return (!(brack < 0 || paren < 0 || sin_q < 0 || dou_q < 0));
}
