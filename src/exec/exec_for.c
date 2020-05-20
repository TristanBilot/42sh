#include "exec.h"
#include "../ast/ast.h"
#include <stdlib.h>

/*
* return -1: error
* return 1: success
* return 0: continue
*/
int perform_for_range(struct range *r, struct node_for *ast)
{
    struct buffer *buf = new_buffer();
    char *left_index = NULL;
    char *right_index = NULL;
    for (size_t i = 0; i < strlen(r->value); i++)
    {

        if (r->value[i] == '{' && is_number(r->value[i + 1]))
        {
            i++;
            /* Left parsing */
            while (r->value[i] != '.')
            {
                if (!is_number(r->value[i]))
                    return -1;
                append_buffer(buf, r->value[i]);
                i++;
            }
            append_buffer(buf, '\0');
            left_index = buf->buf;

            buf = new_buffer();
            if (r->value[i + 1] != '.')
                return -1;
            i += 2;

            /* Right parsing */
            while (r->value[i] != '}')
            {
                append_buffer(buf, r->value[i]);
                i++;
            }
            append_buffer(buf, '\0');
            right_index = buf->buf;
            /* Perform loop */
            int j = atoi(left_index);
            int end = atoi(right_index);
            for (; j <= end; j++)
            {
                char *tmp = xmalloc(MAX_STR_LEN);
                my_itoa(j, tmp);
                put_var(var_storage, ast->variable_name, tmp);
                cont.from_loop = true;
                cont.current_loop++;
                if (exec_node_do_group(ast->body))
                    return -1;
                if (cont.current_loop > 0)
                    cont.current_loop--;
                if (cont.current_loop == 0)
                    cont.from_loop = false;
            }
            del_var(var_storage, ast->variable_name);
            return 1;
        }
    }
    return 0;
}

bool perform_for_enumeration(struct node_for *ast, int len_range)
{
    struct node_for *tmp = ast;
    struct range *r = tmp->range;
    for (int i = 0; i < len_range; i++)
    {
        put_var(var_storage, tmp->variable_name, r->value);
        cont.from_loop = true;
        cont.current_loop++;
        if (exec_node_do_group(ast->body))
            return true;
        if (cont.current_loop > 0)
            cont.current_loop--;
        if (cont.current_loop == 0)
            cont.from_loop = false;
        r = r->next;
    }
    del_var(var_storage, tmp->variable_name);
    return false;
}
