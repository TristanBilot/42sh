#ifndef VAR_STORAGE_H
#define VAR_STORAGE_H

#include <stdbool.h>
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"

#define STORAGE_SIZE 2048

struct var_storage *var_storage;

enum var_type {
    VAR_INT,
    VAR_FLOAT,
    VAR_STRING,
    VAR_ERROR
};

struct variable {
    char *key;
    char *value;
    enum var_type type;
};

struct var_storage {
    struct variable **variables;
};

struct var_storage *new_var_storage(void);
bool var_exists(char *key);
enum var_type get_var_type(char *value);
bool put_var(char *key, char *val);
struct variable *get_var(char *key);
char *get_value(char *key);

#endif /* VAR_STORAGE_H */
