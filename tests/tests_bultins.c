#include <criterion/criterion.h>

#include "../exec/commands.h"

Test(export, simple_export)
{
    char **args = NULL;
    export(args);
    int i = 0;
    for (; environ[i + 1]; i++);
    cr_asser(environ)
}