#include <criterion/criterion.h>
#include "history/history.h"
#include "utils/string_utils.h"

Test(history, basic)
{
    struct history *history = open_history();
    append_history_command(history, "ls");
    append_history_command(history, "pwd");
    append_history_command(history, "echo -e wait");

    cr_assert(is(get_next_history(history), "echo -e wait"));
    cr_assert(is(get_next_history(history), "pwd"));
    cr_assert(is(get_next_history(history), "ls"));

    cr_assert(is(get_prev_history(history), "pwd"));
    cr_assert(is(get_prev_history(history), "echo -e wait"));
    free_history(history);
}
