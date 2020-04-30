#include <criterion/criterion.h>
#include "var_storage.h"
#include "../src/garbage_collector/garbage_collector.h"

Test(var_storage, basic_operation)
{
    new_var_storage();
    new_garbage_collector();

    put_var("key", "val");
    struct variable *var = get_var("key");
    cr_assert(var != NULL);
    cr_assert(is(var->key, "key"));
    cr_assert(is(var->value, "val"));
    free_var_storage();
    free_garbage_collector();
    free(garbage_collector);
}

Test(var_storage, unknown_key)
{
    new_var_storage();
    new_garbage_collector();

    struct variable *var = get_var("key");
    cr_assert(!var);
    free_var_storage();
    free_garbage_collector();
    free(garbage_collector);
}

Test(var_storage, hard_operations)
{
    new_var_storage();
    new_garbage_collector();
    struct variable *var = NULL;

    put_var("key", "val");
    put_var("key", "123");
    var = get_var("key");
    cr_assert(is(var->value, "123"));
    cr_assert(var->type == VAR_INT);

    put_var("key2", "");
    var = get_var("key2");
    cr_assert(is(var->value, ""));
    cr_assert(var->type == VAR_STRING);
    free_var_storage();
    free_garbage_collector();
    free(garbage_collector);
}

Test(var_storage, types)
{
    new_var_storage();
    new_garbage_collector();
    struct variable *var = NULL;

    put_var("keyX", "val");
    var = get_var("keyX");
    cr_assert(is(var->key, "keyX"));
    cr_assert(is(var->value, "val"));
    cr_assert(var->type == VAR_STRING);

    put_var("key2", "1234");
    var = get_var("key2");
    cr_assert(is(var->key, "key2"));
    cr_assert(is(var->value, "1234"));
    cr_assert(var->type == VAR_INT);

    put_var("key3", "0.42");
    var = get_var("key3");
    cr_assert(is(var->key, "key3"));
    cr_assert(is(var->value, "0.42"));
    cr_assert(var->type == VAR_FLOAT);

    put_var("key4", "0abc");
    var = get_var("key4");
    cr_assert(is(var->key, "key4"));
    cr_assert(is(var->value, "0abc"));
    cr_assert(var->type == VAR_STRING);

    put_var("key5", "0.0.1");
    var = get_var("key5");
    cr_assert(is(var->key, "key5"));
    cr_assert(is(var->value, "0.0.1"));
    cr_assert(var->type == VAR_STRING);

    put_var("key6", "23212g3");
    var = get_var("key6");
    cr_assert(is(var->key, "key6"));
    cr_assert(is(var->value, "23212g3"));
    cr_assert(var->type == VAR_STRING);
    free_var_storage();
    free_garbage_collector();
    free(garbage_collector);
}
