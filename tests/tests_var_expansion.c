#include <criterion/criterion.h>
#include "expansion/expansion.h"

struct var_storage *alias_storage;
struct var_storage *var_storage;
struct file_manager *file_manager;

Test(var_storage, basic_operation)
{
    put_var("key", "val");
    struct variable *var = get_var("key");
    cr_assert(var != NULL);
    cr_assert(is(var->key, "key"));
    cr_assert(is(var->value, "val"));
}
