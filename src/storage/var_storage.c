#include "var_storage.h"
#include <stdio.h>

struct var_storage *new_var_storage(void)
{
    struct var_storage *storage = malloc(sizeof(struct var_storage));
    storage->variables = calloc(STORAGE_SIZE, sizeof(struct variable));
    return storage;
}

void free_var_storage(struct var_storage *storage)
{
    for (int i = 0; i < STORAGE_SIZE; i++)
        if (storage->variables[i])
        {
            if (storage->variables[i]->value)
                free(storage->variables[i]->value);
            free(storage->variables[i]);
        }
    free(storage->variables);
    free(storage);
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

bool var_exists(struct var_storage *storage, char *key)
{
    if (!key)
        return false;
    int h = hash(key);
    return storage->variables[h] != NULL;
}

/*
* return false only if the var_type returnes is VAR_ERROR
*/
bool put_var(struct var_storage *storage, char *key, char *val)
{
    if (!key || !val)
        return true;
    int h = hash(key);
    if (var_exists(storage, key))
    {
        storage->variables[h]->value = \
            realloc(storage->variables[h]->value, strlen(val) + 1);
        strcpy(storage->variables[h]->value, val);
        storage->variables[h]->type = get_var_type(val);
        return storage->variables[h]->type != VAR_ERROR;
    }
    struct variable *new = malloc(sizeof(struct variable));
    new->value = malloc(strlen(val) + 1);
    strcpy(new->value, val);
    new->key = key;
    new->type = get_var_type(val);
    storage->variables[h] = new;
    return new->type != VAR_ERROR;
}

bool del_var(struct var_storage *storage, char *key)
{
    int h = hash(key);
    if (storage->variables[h])
    {
        free(storage->variables[h]->value);
        free(storage->variables[h]);
        storage->variables[h] = NULL;
        return true;
    }
    else
        return false;
}

struct variable *get_var(struct var_storage *storage, char *key)
{
    int h = hash(key);
    return storage->variables[h];
}

char *get_value(struct var_storage *storage, char *key)
{
    int h = hash(key);
    return storage->variables[h]->value;
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
