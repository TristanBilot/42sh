/**
** \file var_storage.h
** \author Team 
** \brief Var storage structures and functions
** \version 0.1
** \date 2020-05-03
** 
** \copyright Copyright (c) 2020
** 
*/
#ifndef VAR_STORAGE_H
#define VAR_STORAGE_H

#include <stdbool.h>
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"

#define STORAGE_SIZE 2048

struct var_storage *alias_storage;
struct var_storage *var_storage;

/* Variables */
enum var_type
{
    VAR_INT,
    VAR_FLOAT,
    VAR_STRING,
    VAR_ERROR
};

struct variable
{
    char *key;
    char *value;
    enum var_type type;
};

struct var_storage
{
    struct variable **variables;
};

struct var_storage *new_var_storage(void);
void free_var_storage(struct var_storage *storage);
bool var_exists(struct var_storage *storage, char *key);
enum var_type get_var_type(char *value);
bool put_var(struct var_storage *storage, char *key, char *val);
void del_var(struct var_storage *storage, char *key);
struct variable *get_var(struct var_storage *storage, char *key);
char *get_value(struct var_storage *storage, char *key);

#endif /* VAR_STORAGE_H */
