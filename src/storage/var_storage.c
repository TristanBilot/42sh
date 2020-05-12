#include "var_storage.h"
#include <stdio.h>

struct var_storage *new_var_storage(void)
{
    var_storage = malloc(sizeof(struct var_storage));
    var_storage->variables = calloc(STORAGE_SIZE, sizeof(struct variable));
    return var_storage;
}

void free_var_storage(void)
{
    for (int i = 0; i < STORAGE_SIZE; i++)
        if (var_storage->variables[i])
        {
            if (var_storage->variables[i]->value)
                free(var_storage->variables[i]->value);
            free(var_storage->variables[i]);
        }
    free(var_storage->variables);
    free(var_storage);
}

int hash(char *key)
{
    int hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    if (hash < 0)
        hash *= -1;
    return hash % STORAGE_SIZE;
}

bool var_exists(char *key)
{
    if (!key)
        return false;
    int h = hash(key);
    return var_storage->variables[h] != NULL;
}

/*
* return false only if the var_type returnes is VAR_ERROR
*/
bool put_var(char *key, char *val)
{
    if (!key || !val)
        return true;
    int h = hash(key);
    if (var_exists(key))
    {
        var_storage->variables[h]->value = \
            realloc(var_storage->variables[h]->value, strlen(val) + 1);
        strcpy(var_storage->variables[h]->value, val);
        var_storage->variables[h]->type = get_var_type(val);
        return var_storage->variables[h]->type != VAR_ERROR;
    }
    struct variable *new = malloc(sizeof(struct variable));
    new->value = malloc(strlen(val) + 1);
    strcpy(new->value, val);
    new->key = key;
    new->type = get_var_type(val);
    var_storage->variables[h] = new;
    return new->type != VAR_ERROR;
}

void del_var(char *key)
{
    int h = hash(key);
    if (var_storage->variables[h])
    {
        free(var_storage->variables[h]->value);
        free(var_storage->variables[h]);
        var_storage->variables[h] = NULL;
    }
}

struct variable *get_var(char *key)
{
    int h = hash(key);
    return var_storage->variables[h];
}

char *get_value(char *key)
{
    int h = hash(key);
    return var_storage->variables[h]->value;
}

enum var_type get_var_type(char *value)
{
    if (!value)
        return VAR_ERROR;

    size_t point_counter = 0;
    bool should_be_number = is_number(value[0]);
    for (size_t i = 0; i < strlen(value); i++)
    {
        if (value[i] == '.')
            point_counter++;
        if (should_be_number && (!is_number(value[i]) && value[i] != '.'))
            return VAR_STRING;
    }
    if (point_counter > 1)
        return VAR_STRING;
        
    if (should_be_number)
        return point_counter == 0 ? VAR_INT : VAR_FLOAT;
    return VAR_STRING;
}
