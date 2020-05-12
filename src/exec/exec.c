#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "../exec/exec.h"
#include "../utils/string_utils.h"
#include "../storage/var_storage.h"
#include "../storage/program_data_storage.h"
#include "../expansion/expansion.h"
#include "../exec/commands.h"
#include "../expansion/my_popen.h"

#define READ_END 0
#define WRITE_END 1
#define STDOUT_FILENO 1
#define STDIN_FILENO 0

#define DEBUG_FLAG false
#define DEBUG(msg) if (DEBUG_FLAG) \
    printf("%s\n", msg);


const struct commands cmd[4] = {
    {"cd", &cd}, 
    {"echo", &echo},
    {"export", &export},
    {NULL, NULL}};

bool extra_command(char **args, char *cmd_name)
{
    for (int i = 0; cmd[i].name; i++)
    {
        if (strcmp(cmd_name, cmd[i].name) == 0)
        {
            cmd[i].function(args);
            return false;
        }
    }
    return true;
}

bool clean_extra_command(void)
{
    char *args[3] = {"echo", "", NULL};
    if ((execvp("echo", args)) == -1)
    {
        err(1, "command not found: %s\n", args[0]);
        return true;
    }
    return false;
}

bool dup_file(char *file, char *flag, int io, int *ptr_fd)
{
    FILE *f = fopen(file, flag);
    if (!f)
        return true;
    int out = fileno(f);
    if (out == -1)
    {
        return true;
    }
    close(io);
    int fd = dup2(out, io);
    *ptr_fd = out;
    if (fd < 0)
    {
        close(out);
        return true;
    }
    // close(fd);
    close(out);
    fclose(f);
    return false;
}

bool manage_redirections(struct tab_redi tab, int *ptr_fd)
{
    // >
    if (!is(tab.great.out, ""))
        if (dup_file(tab.great.out, "w+", STDOUT_FILENO, ptr_fd))
            return true;
    if (!is(tab.great.in, ""))
        if (dup_file(tab.great.in, "w+", STDIN_FILENO, ptr_fd))
            return true;
    if (!is(tab.great.err, ""))
        if (dup_file(tab.great.err, "w+", STDERR_FILENO, ptr_fd))
            return true;
    
    //  < 
    if (!is(tab.less.in, ""))
        if (dup_file(tab.less.in, "r", STDIN_FILENO, ptr_fd))
            return true;
    if (!is(tab.less.out, ""))
        if (dup_file(tab.less.out, "r", STDOUT_FILENO, ptr_fd))
            return true;
    if (!is(tab.less.err, ""))
        if (dup_file(tab.less.err, "r", STDERR_FILENO, ptr_fd))
            return true;

    // >>
    if (!is(tab.dgreat.in, ""))
        if (dup_file(tab.dgreat.in, "a+", STDIN_FILENO, ptr_fd))
            return true;
    if (!is(tab.dgreat.out, ""))
        if (dup_file(tab.dgreat.out, "a+", STDOUT_FILENO, ptr_fd))
            return true;
    if (!is(tab.dgreat.err, ""))
        if (dup_file(tab.dgreat.err, "a+", STDERR_FILENO, ptr_fd))
            return true;

    // <<
    if (!is(tab.dless.in, ""))
        if (dup_file(tab.dless.in, "a+", STDIN_FILENO, ptr_fd))
            return true;
    if (!is(tab.dless.out, ""))
        if (dup_file(tab.dless.out, "a+", STDOUT_FILENO, ptr_fd))
            return true;
    if (!is(tab.dless.err, ""))
        if (dup_file(tab.dless.err, "a+", STDERR_FILENO, ptr_fd))
            return true;
    
    return false;
}

bool execute(char **args, struct tab_redi tab)
{
    int *fd = xmalloc(sizeof(int));
    *fd = 1;
    int *ptr_fd;
    ptr_fd = fd;
    DEBUG("EXECUTE")
    if (manage_redirections(tab, ptr_fd))
        return true;
    if ((execvp(args[0], args)) == -1)
    {
        err(1, "command not found: %s\n", args[0]);
        return true;
    }
    close(*fd);
    return false;
}

static bool execute_with_fork(char **args, struct tab_redi tab, char *cmd_name)
{
    DEBUG("EXECUTE WITH FORK")
    int status = 0;
    int child = 0;
    int *fd = xmalloc(sizeof(int));
    *fd = 1;
    int *ptr_fd;
    ptr_fd = fd;
    int save_out = dup(1);
    int save_in = dup(0);
    int save_err = dup(2);
    
    if (args[0] && strcmp(args[0], "exit") == 0)
        exit_shell(args);
    if (manage_redirections(tab, ptr_fd))
    {
        err(1, "redirection ");
        update_last_status(1);
        // WEXITSTATUS(status) = 1;
        return true;
    }

    // if (args[0] == NULL)
    // {
    //     if (!extra_command(args, cmd_name))
    //     {
    //         dup2(save_in, 0);
    //         dup2(save_out, 1);
    //         dup2(save_err, 2);
    //         return false;
    //     }
    // }
    if (!extra_command(args, cmd_name))
    {
        dup2(save_in, 0);
        dup2(save_out, 1);
        dup2(save_err, 2);
        //return false;
        return false;

    }
    else
    {
    
        if ((child = fork()) == -1)
            return true;
        if (child == 0)
        {
            if ((execvp(args[0], args)) == -1)
            {
                err(1, "command not found: %s\n", args[0]);
                return true;
            }
            return false;
        }
        else
        {
            wait(&status);
            if (WIFEXITED(status))
            {
                //exit(status);
                update_last_status(WEXITSTATUS(status));
                
                return WEXITSTATUS(status) >= 1;
            }
        }
    }
    return false;
    
}

bool exec_node_input(struct node_input *ast)
{
    DEBUG("INPUT")
    if(!ast)
        return false;
    return ast->node_list ? exec_node_list(ast->node_list) : false;
}
bool exec_node_list(struct node_list *ast)
{
    DEBUG("LIST")
    struct node_list *c = ast;
    if (c->next_sibling)
    {
        exec_node_and_or(c->and_or);
    }
    else
        return exec_node_and_or(c->and_or);
    int state = 0;
    while (c->next_sibling)
    {
        state = exec_node_and_or(c->next_sibling->and_or);
        c = c->next_sibling;
    }
    return state;
}
bool exec_node_and_or(struct node_and_or *ast)
{
    DEBUG("AND_OR")
    if (ast->right)
    {
        if (ast->type == OR)
        {
            if (ast->is_final)
            {
                if (exec_node_pipeline(ast->left.pipeline))
                    return exec_node_pipeline(ast->right);
                return false;
            }
            else
            {
                if (exec_node_and_or(ast->left.and_or))
                    return exec_node_pipeline(ast->right);
                return false;
            }
        }
        if (ast->type == AND)
        {
            if (ast->is_final)
            {
                if (!exec_node_pipeline(ast->left.pipeline))
                    return exec_node_pipeline(ast->right);
                return true;
            }
            else
            {
                if (!exec_node_and_or(ast->left.and_or))
                    return exec_node_pipeline(ast->right);
                return true;
            }
        }
    }
    else
    {
        return exec_node_pipeline(ast->left.pipeline);
        /*if (ast->is_final)
            exec_node_pipeline(ast->left.pipeline);
        else
            exec_node_and_or(ast->left.and_or);*/
    }
    return false;
}

bool exec_node_pipeline(struct node_pipeline *ast)
{
    DEBUG("PIPELINE")
    struct node_pipeline *c = ast;
    int state = false;

    if (!c->next_sibling)
    {
        if (ast->is_not)
            return !exec_node_command(c->command, true);
        return exec_node_command(c->command, true);
    }
    else
    {
        int pid[NB_MAX_PIPE] = {-1};
        int fd[NB_MAX_PIPE][2];
        int status;
        int nb = 0;
        
        while (c)
        {
            status = pipe(fd[nb]);
            if (status < 0)
            {
                ERROR("pipe() failed (node_pipeline)")
            }
            pid[nb] = fork();
            if (pid[nb] < 0)
            {
                ERROR("fork() failed (node_pipeline)");
            }
            else if (pid[nb])
            {
                if (c->next_sibling)
                {
                    c = c->next_sibling;
                    nb++;
                    continue;
                }
                for (int i = 0; i <= nb; i++)
                {
                    close(fd[i][READ_END]);
                    close(fd[i][WRITE_END]);
                }
                for (int i = 0; i <= nb; i++)
                {
                    waitpid(pid[i], &status, 0);
                    if (status > 0) /* failure > 0, success = 0 */
                        state = true;
                }
                update_last_status(state);
                // exit(is_not ? !state : state); /* only for tests */
                return ast->is_not ? !state : state;
            }
            else
            {
                if (nb == 0)
                    close(fd[0][READ_END]);
                else
                    dup2(fd[nb-1][READ_END], 0);
                if (!c->next_sibling)
                    close(fd[nb][WRITE_END]);
                else
                    dup2(fd[nb][WRITE_END],1);
                for (int i = 0; i <= nb; i++)
                {
                    close(fd[i][READ_END]);
                    close(fd[i][WRITE_END]);
                }
                exit(exec_node_command(c->command, false));
            }
        }
    }
    return false;
}

bool exec_node_command(struct node_command *ast, bool with_fork)
{
    DEBUG("COMMAND")
    int state = false;
    if (ast->type == SIMPLE_COMMAND)
        return exec_node_simple_command(ast->command.simple_command, with_fork);
    
    else if (ast->type == SHELL_COMMAND)
    {
        state = exec_node_shell_command(ast->command.shell_command);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            r = r->next;
        }
    }
    else if (ast->type == FUNCDEC)
    {
        state = exec_node_funcdec(ast->command.funcdec);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            r = r->next;
        }
    }
    return state;
}

struct tab_redi init_tab_redi(struct tab_redi tab)
{
    tab.great.out = "";
    tab.great.in = "";
    tab.great.err = "";
    tab.less.out = "";
    tab.less.in = "";
    tab.less.err = "";
    tab.dgreat.out = "";
    tab.dgreat.in = "";
    tab.dgreat.err = "";
    tab.dless.out = "";
    tab.dless.in = "";
    tab.dless.err = "";
    tab.dlessdash.out = "";
    tab.dlessdash.in = "";
    tab.dlessdash.err = "";
    tab.greatand.out = "";
    tab.greatand.in = "";
    tab.greatand.err = "";
    tab.lessand.out = "";
    tab.lessand.in = "";
    tab.lessand.err = "";
    tab.lessgreat.out = "";
    tab.lessgreat.in = "";
    tab.lessgreat.err = "";
    tab.clobber.out = "";
    tab.clobber.in = "";
    tab.clobber.err = "";
    return tab;
}

struct tab_redi append_tab_redi(struct tab_redi tab, struct node_redirection *e)
{
    if (is(type_to_str(e->type), "TOK_GREAT"))
    {
        if (!e->left || is(e->left, "") || is(e->left, "1"))
            tab.great.out = e->right;
        else if (is(e->left, "0"))
            tab.great.in = e->right;
        else if (is(e->left, "2"))
            tab.great.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_LESS"))
    {
        if (is(e->left, "") || is(e->left, "0"))
            tab.less.in = e->right;
        else if (is(e->left, "1"))
            tab.less.out = e->right;
        else if (is(e->left, "2"))
            tab.less.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_DLESS"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.dless.out = e->right;
        else if (is(e->left, "0"))
            tab.dless.in = e->right;
        else if (is(e->left, "2"))
            tab.dless.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_DLESSDASH"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.dlessdash.out = e->right;
        else if (is(e->left, "0"))
            tab.dlessdash.in = e->right;
        else if (is(e->left, "2"))
            tab.dlessdash.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_LESSGREAT"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.lessgreat.out = e->right;
        else if (is(e->left, "0"))
            tab.lessgreat.in = e->right;
        else if (is(e->left, "2"))
            tab.lessgreat.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_LESSAND"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.lessand.out = e->right;
        else if (is(e->left, "0"))
            tab.lessand.in = e->right;
        else if (is(e->left, "2"))
            tab.lessand.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_DGREAT"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.dgreat.out = e->right;
        else if (is(e->left, "0"))
            tab.dgreat.in = e->right;
        else if (is(e->left, "2"))
            tab.dgreat.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_GREATAND"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.greatand.out = e->right;
        else if (is(e->left, "0"))
            tab.greatand.in = e->right;
        else if (is(e->left, "2"))
            tab.greatand.err = e->right;
    }
    else if (is(type_to_str(e->type), "TOK_CLOBBER"))
    {
        if (is(e->left, "") || is(e->left, "1"))
            tab.clobber.out = e->right;
        else if (is(e->left, "0"))
            tab.clobber.in = e->right;
        else if (is(e->left, "2"))
            tab.clobber.err = e->right;
    }
    return tab;
}

bool exec_node_simple_command(struct node_simple_command *ast, bool with_fork)
{
    DEBUG("SIMPLE_COMMAND")
    struct node_prefix *p = ast->prefixes;
    struct node_element *e = ast->elements;
    struct tab_redi tab = init_tab_redi(tab);
    char *args[256];

    while (p)
    {
        if (p->type == ASSIGMENT_WORD)
        {
            char *key = p->prefix.assigment_word->variable_name;
            char *val = p->prefix.assigment_word->value;
            put_var(key, val);
        }
        else
            tab = append_tab_redi(tab, p->prefix.redirection);
        p = p->next;
    }

    if (e) /* link the root node to the first element (ex: echo) */
    {
        int size = 0;
        if (e->type == TOKEN_REDIRECTION)
            tab = append_tab_redi(tab, e->element.redirection);
        else
            args[size++] = e->element.word;
        while (e->next) /* link each child between them */
        {
            e = e->next;
            if (e->type == TOKEN_REDIRECTION)
            {
                tab = append_tab_redi(tab, e->element.redirection);
            }
            else
            {
                char *expanded_word = substitute(e->element.word);
                if (!expanded_word)
                    return true;
                args[size++] = expanded_word;
            }
        }
        args[size++] = NULL;
        bool ret = false;

        if (ast->to_export)
            ret = with_fork ? execute_with_fork(args, tab, "export") : execute(args, tab);
        else
        {
            ret = with_fork ? execute_with_fork(args, tab, args[0]) : execute(args, tab);
        }
        return ret;
    }
    if (ast->prefixes != NULL && ast->to_export == true)
    {
        struct variable *tmp = NULL;
        int i = 0;
        
        struct node_prefix *prefix = ast->prefixes;
        while(prefix)
        {
            if (var_exists(prefix->prefix.assigment_word->variable_name))
            {
                tmp = get_var(prefix->prefix.assigment_word->variable_name);
                char *str = xmalloc((strlen(tmp->key) + 1 + strlen(tmp->value) + 1) * sizeof(char));
                str = tmp->key;
                strcat(str, "=");
                strcat(str, tmp->value);
                args[i] = str;
                i += 1;
            }

            prefix = prefix->next;
        }
        args[i] = NULL;
        i = 0;
        return with_fork ? execute_with_fork(args, tab, "export") : execute(args, tab);
    }
    args[0] = NULL;
    if (ast->to_export){
        return with_fork ? execute_with_fork(args, tab, "export") : execute(args, tab);
    }
    return false;
}

bool exec_node_shell_command(struct node_shell_command *ast)
{
    DEBUG("SHELL_COMMAND")
    switch (ast->type)
    {
    case C_BRACKETS:
        exec_node_compound_list(ast->shell.compound_list);
        break;
    case PARENTHESIS:
        exec_node_compound_list(ast->shell.compound_list);
        break;
    case RULE:
        if (ast->shell_type == WHILE)
            exec_node_while(ast->shell.rule_while);
        if (ast->shell_type == FOR)
        {
            exec_node_for(ast->shell.rule_for);
        }
        if (ast->shell_type == UNTIL)
            exec_node_until(ast->shell.rule_until);
        if (ast->shell_type == CASE)
            exec_node_case(ast->shell.rule_case);
        if (ast->shell_type == IF)
            exec_node_if(ast->shell.rule_if);
        break;
    default:
        break;
    }
    return false;
}
bool exec_node_funcdec(struct node_funcdec *ast)
{
    DEBUG("FUNCDEC")
    exec_node_shell_command(ast->shell_command);
    return false;
}


bool exec_node_compound_list(struct node_compound_list *ast)
{
    DEBUG("COMPOUND_LIST")
    struct node_compound_list *c = ast;
    while (c)
    {
        if (exec_node_and_or(c->and_or))
            return true;
        c = c->next_sibling;
    }
    return false;
}
bool exec_node_while(struct node_while *ast)
{
    DEBUG("WHILE")
    bool state = false;
    while (!exec_node_compound_list(ast->condition))
        state = exec_node_do_group(ast->body);
    if (!state)
        update_last_status(0);
    return false;
}
bool exec_node_until(struct node_until *ast)
{
    DEBUG("UNTIL")
    while (exec_node_compound_list(ast->condition))
        exec_node_do_group(ast->body);
    return false;
}
bool exec_node_case(struct node_case *ast)
{
    DEBUG("CASE")
    if (ast->is_case_clause)
        exec_node_case_clause(ast->case_clause, ast->word);
    return false;
}

bool exec_node_if(struct node_if *ast)
{
    DEBUG("IF")
    if (!exec_node_compound_list(ast->condition))
        return exec_node_compound_list(ast->if_body);
    else if (ast->else_clause)
        return exec_node_else_clause(ast->else_clause);
    return false;
}

bool exec_node_elif(struct node_if *ast)
{
    DEBUG("ELIF")
    if (!exec_node_compound_list(ast->condition))
        return exec_node_compound_list(ast->if_body);
    else if (ast->else_clause)
        return exec_node_else_clause(ast->else_clause);
    return false;
}

/*
* for i in 1 2 3 4 5
* for i in $(command)
* for i in {1..5}
*/
bool exec_node_for(struct node_for *ast)
{
    DEBUG("FOR")
    struct range *r = ast->range;
    int len_range = 0;
    // struct range *tmp = (void *) ast;
    int state = false;
    while (r)
    {
        if (len_range == 0)
        {
            state = perform_for_range(r, ast);
            if (state == -1)
                return true;
            if (state == 1)
                return false;
        }

        len_range++;
        r = r->next;
    }
    return perform_for_enumeration(ast, len_range);
    //exec_node_do_group(ast->body);
    //return false;
}
bool exec_node_else_clause(struct node_else_clause *ast)
{
    DEBUG("ELSE_CLAUSE")
    if (ast->type == ELSE)
        exec_node_compound_list(ast->clause.else_body);
    else if (ast->type == ELIF)
        exec_node_elif(ast->clause.elif);
    return false;
}
bool exec_node_do_group(struct node_do_group *ast)
{
    DEBUG("DO_GROUP")
    return exec_node_compound_list(ast->body);
}
bool exec_node_case_clause(struct node_case_clause *ast, char *word_to_found)
{
    DEBUG("CASE_CLAUSE")
    struct node_case_clause *c = ast;
    if (!c || !c->case_item)
        return true;
    // if (!exec_node_case_item(c->case_item, word_to_found))
    //         return false;
    // while (c->next)
    // {
    //     c = c->next;
    //     if (!exec_node_case_item(c->case_item, word_to_found))
    //         return false;
    // }

    while (c)
    {
        if (!exec_node_case_item(c->case_item, word_to_found))
            return false;
        c = c->next;
    }
    return false;
}
bool exec_node_case_item(struct node_case_item *ast, char *word_to_found)
{
    DEBUG("CASE_ITEM")
    while (ast->words)
    {
        if (strcmp(word_to_found, ast->words->word) == 0)
        {
            if (ast->compound_list)
            {
                exec_node_compound_list(ast->compound_list);
                return false;
            }
            else
                return false;
        }
        else
            ast->words = ast->words->next;
    }
    return true;
}