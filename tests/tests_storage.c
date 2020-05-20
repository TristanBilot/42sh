#include <criterion/criterion.h>
#include "var_storage.h"
#include "../src/garbage_collector/garbage_collector.h"

Test(var_storage, basic_operation)
{
    new_garbage_collector();
    var_storage = new_var_storage();
    put_var(var_storage, "key", "val");
    struct variable *var = get_var(var_storage, "key");
    cr_assert(var != NULL);
    cr_assert(is(var->key, "key"));
    cr_assert(is(var->value, "val"));
    free_var_storage(var_storage);
    free_garbage_collector();
    free(garbage_collector);

}

Test(var_storage, unknown_key)
{
    new_garbage_collector();
    var_storage = new_var_storage();

    struct variable *var = get_var(var_storage, "key");
    cr_assert(!var);
    free_var_storage(var_storage);
    free_garbage_collector();
    free(garbage_collector);

}

Test(var_storage, hard_operations)
{
    new_garbage_collector();
    var_storage = new_var_storage();
    struct variable *var = NULL;

    put_var(var_storage, "key", "val");
    put_var(var_storage, "key", "123");
    var = get_var(var_storage, "key");
    cr_assert(is(var->value, "123"));
    cr_assert(var->type == VAR_INT);

    put_var(var_storage, "key2", "");
    var = get_var(var_storage, "key2");
    cr_assert(is(var->value, ""));
    cr_assert(var->type == VAR_STRING);
    free_var_storage(var_storage);
    free_garbage_collector();
    free(garbage_collector);
}

Test(var_storage, types)
{
    new_garbage_collector();
    var_storage = new_var_storage();
    struct variable *var = NULL;

    put_var(var_storage, "keyX", "val");
    var = get_var(var_storage, "keyX");
    cr_assert(is(var->key, "keyX"));
    cr_assert(is(var->value, "val"));
    cr_assert(var->type == VAR_STRING);

    put_var(var_storage, "key2", "1234");
    var = get_var(var_storage, "key2");
    cr_assert(is(var->key, "key2"));
    cr_assert(is(var->value, "1234"));
    cr_assert(var->type == VAR_INT);

    put_var(var_storage, "key3", "0.42");
    var = get_var(var_storage, "key3");
    cr_assert(is(var->key, "key3"));
    cr_assert(is(var->value, "0.42"));
    cr_assert(var->type == VAR_FLOAT);

    put_var(var_storage, "key4", "0abc");
    var = get_var(var_storage, "key4");
    cr_assert(is(var->key, "key4"));
    cr_assert(is(var->value, "0abc"));
    cr_assert(var->type == VAR_STRING);

    put_var(var_storage, "key5", "0.0.1");
    var = get_var(var_storage, "key5");
    cr_assert(is(var->key, "key5"));
    cr_assert(is(var->value, "0.0.1"));
    cr_assert(var->type == VAR_STRING);

    put_var(var_storage, "key6", "23212g3");
    var = get_var(var_storage, "key6");
    cr_assert(is(var->key, "key6"));
    cr_assert(is(var->value, "23212g3"));
    cr_assert(var->type == VAR_STRING);
    free_var_storage(var_storage);
    free_garbage_collector();
    free(garbage_collector);
}
