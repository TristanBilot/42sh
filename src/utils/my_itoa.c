#include <stdlib.h>
#include <stdbool.h>

int number_digits(int n)
{
    int cpt = 0;
    if (n < 0)
        cpt++;
    while (n != 0)
    {
        n /= 10;
        cpt++;
    }
    return cpt;
}

int power(int x, int y)
{
    if (y == 0)
        return 1;
    if (y == 1)
        return x;
    int cpt = x;
    for (int i = 1; i < y; ++i)
        cpt *= x;
    return cpt;
}

char *my_itoa(int value, char *s)
{
    bool is_neg = value < 0;
    if (value == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    if (is_neg)
    {
        value = -value;
        s[0] = '-';
    }
    int nb_digits = number_digits(value);
    int current_digit;
    int cpt = is_neg;
    for (int i = nb_digits - 1; i >= 0; i--)
    {
        current_digit = (value / power(10, i)) % 10;
        char ascii_digit = '0' + current_digit;
        s[cpt++] = ascii_digit;
    }
    s[nb_digits + is_neg] = '\0';
    return s;
}
