#include "../exec/exec.h"
#include "../utils/string_utils.h"
#include "../var_storage/var_storage.h"
#include "../expansion/expansion.h"
#include <fcntl.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define STDOUT_FILENO 1
#define STDIN_FILENO 0
#define DEBUG_FLAG false
#define DEBUG(msg) if (DEBUG_FLAG) \
                        printf("%s\n", msg);
bool execute(char **args)
{
    if ((execvp(args[0], args)) == -1)
    {
        err(1, "command not found: %s\n", args[0]);
        return true;
    }
    return false;
}

bool execute_with_fork(char **args, struct tab_redirection *tab)
{
    int	status = 0;
    int	child = 0;
    if ((child = fork()) == -1)
        return true;
    if (child == 0)
    {
        // printf("just entered stdout :%s\n", tab->stdout->dest);
        // while (tab && tab->stdout)
        // {
        //     printf("enter while\n"); // ne s'affiche plus quand on affiche tab->stdout->dest
            // int out = open(tab->stdin->dest, O_WRONLY | O_CREAT);
            // printf("ok\n");
            // if (out == -1)
            // {
            //     printf("RET1\n");
            //     return true;
            // }
            // int fd = dup2(out, STDOUT_FILENO);
            // printf("fd %d\n", fd);
            // if (fd != 0)
            // {
            //     printf("RET2\n");
            //     return true;
            // }
            // close(out);
        //     tab->stdin = tab->stdin->next;
        //     printf ("finish while\n");
        // }
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
            // printf("exit status = %d\n", WEXITSTATUS(status));
            update_last_status(WEXITSTATUS(status));
            return WEXITSTATUS(status) == 1; /* 1 = no output in stdout */
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
        if (c->type == SEMI)
        {
            // fprintf(f, "\tnode_%p [label=SEMI];\n", (void *)c);
        }
        else if (c->type == SEPAND)
        {
            // fprintf(f, "\tnode_%p [label=SEPAND];\n", (void *)c);
        }
        exec_node_and_or(c->and_or);
    }
    else
        exec_node_and_or(c->and_or);
    while (c->next_sibling)
    {
        if (c->next_sibling->next_sibling)
        {
            if (c->next_sibling->type == SEMI)
            {
                // fprintf(f, "\tnode_%p [label=SEMI];\n", (void *)c->next_sibling);
                // fprintf(f, "\tnode_%p -> node_%p;\n", (void *) c, (void *) c->next_sibling);
            }
            else if (c->next_sibling->type == SEPAND)
            {
                // fprintf(f, "\tnode_%p [label=SEPAND];\n", (void *)c->next_sibling);
                // fprintf(f, "\tnode_%p -> node_%p;\n", (void *) c, (void *) c->next_sibling);
            }
            exec_node_and_or(c->next_sibling->and_or);
        }
        else
            exec_node_and_or(c->next_sibling->and_or);
        c = c->next_sibling;
    }
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
            }
            else
            {
                if (exec_node_and_or(ast->left.and_or))
                    return exec_node_pipeline(ast->right);
            }
        }
        if (ast->type == AND)
        {
            if (ast->is_final)
            {
                if (!exec_node_pipeline(ast->left.pipeline))
                    return exec_node_pipeline(ast->right);
            }
            else
            {
                if (!exec_node_and_or(ast->left.and_or))
                    return exec_node_pipeline(ast->right);
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
}

bool exec_node_pipeline(struct node_pipeline *ast)
{
    DEBUG("PIPELINE")
    struct node_pipeline *c = ast;
    bool state = false;

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
        int status;// T'es la ??
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
    bool state = false;
    if (ast->type == SIMPLE_COMMAND)
        return exec_node_simple_command(ast->command.simple_command, with_fork);
    else if (ast->type == SHELL_COMMAND)
    {
        state = exec_node_shell_command(ast->command.shell_command);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            state = exec_node_redirection(r);
            r = r->next;
        }
    }
    else if (ast->type == FUNCDEC)
    {
        state = exec_node_funcdec(ast->command.funcdec);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            state = exec_node_redirection(r);
            r = r->next;
        }
    }
    return state;
}

struct tab_redirection *init_tab_redirection(void)
{
    struct tab_redirection *new = xcalloc(1, sizeof(struct tab_redirection));
    return new;
}

bool exec_node_simple_command(struct node_simple_command *ast, bool with_fork)
{
    DEBUG("SIMPLE_COMMAND")
    struct node_prefix *p = ast->prefixes;
    struct node_element *e = ast->elements;
    // fprintf(f, "\tnode_%p [label=SIMPLECOMMAND];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    /*if(p->type == 0)
        printf("prefix is redirection\n");
    if(e->type == 0)
        printf("element is redirection\n");*/
    while (p)
    {
        exec_node_prefix(p);

        p = p->next;
    }
    if (e) /* link the root node to the first element (ex: echo) */
    {
        char *args[256]; // love satan
        struct tab_redirection *tab = init_tab_redirection();
        int size = 0;
        if (e->type == REDIRECTION)
        {
            printf("e->type = redirection\n");
            if (!e->element.redirection->left || is(e->element.redirection->left, "1"))
                tab->stdout = append_list_stdout(tab, e->element.redirection->right);
            else if (is(e->element.redirection->left, "0"))
                tab->stdin = append_list_stdin(tab, e->element.redirection->right);
            else if (is(e->element.redirection->left, "2"))
                tab->stderr = append_list_stderr(tab, e->element.redirection->right);
        }
        else
            args[size++] = e->element.word;
        while (e->next) /* link each child between them */
        {
            e = e->next;
            if (e->type == REDIRECTION)
            {
                printf("e->type = redirection\n");
                if (!e->element.redirection->left || is(e->element.redirection->left, "") || is(e->element.redirection->left, "1"))
                {
                    printf("add one %s\n", e->element.redirection->right);
                    tab->stdout = append_list_stdout(tab, e->element.redirection->right);
                    printf("added\n");
                }
                else if (is(e->element.redirection->left, "0"))
                    tab->stdin = append_list_stdin(tab, e->element.redirection->right);
                else if (is(e->element.redirection->left, "2"))
                    tab->stderr = append_list_stderr(tab, e->element.redirection->right);
            }
            else
            {
                char *expanded_word = perform_var_expansion(e->element.word);
                args[size++] = expanded_word;
                // args[size++] = e->element.word;
            }
        }
        args[size++] = NULL;
        bool ret = with_fork ? execute_with_fork(args, tab) : execute(args); // add tab in parameters
        free_tab(tab);
        return ret;
    }
}

bool exec_node_shell_command(struct node_shell_command *ast)
{
    DEBUG("SHELL_COMMAND")
    switch (ast->type)
    {
    case C_BRACKETS:
        // fprintf(f, "\tnode_%p [label=C_BRACKET];\n", (void *)ast);
        // if (node)
            // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        exec_node_compound_list(ast->shell.compound_list);
        break;
    case PARENTHESIS:
        // fprintf(f, "\tnode_%p [label=PARENTHESIS];\n", (void *)ast);
        // if (node)
            // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        exec_node_compound_list(ast->shell.compound_list);
        break;
    case RULE:
        if (ast->shell_type == WHILE)
            exec_node_while(ast->shell.rule_while);
        if (ast->shell_type == FOR)
        {
            printf("enter for\n");
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
}
bool exec_node_funcdec(struct node_funcdec *ast)
{
    DEBUG("FUNCDEC")
    if (ast->is_function)
    {
        // fprintf(f, "\tnode_%p [label=FUNCTION];\n", (void *)ast);
    }
    else
        // fprintf(f, "\tnode_%p [label=FUNCTION_CALL];\n", (void *)ast);
    // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        
    // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->function_name, ast->function_name);
    // fprintf(f, "\tnode_%p -> node_%p;\n", (void *)ast, (void *)ast->function_name);
    exec_node_shell_command(ast->shell_command);
}

bool exec_node_redirection(struct node_redirection *ast)
{
    DEBUG("REDIRECTION")
    // char *redirection = type_to_str(ast->type);
    // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, redirection);
    // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    // printf("0\n");
    int status;
    int pid = fork();
    if(pid < 0)
        exit(1);
    else if(pid == 0)
    {
        if (ast->left && is(ast->left, ""))
        {
         // cas du echo test > toto.txt
            printf("1\n");
            if (ast->right && !is(ast->right, ""))
            {
                printf("2\n");
                if(ast->type == TOK_GREAT)
                {
                    // int out = open(ast->right, O_WRONLY | O_CREAT);
                    // if (out == -1)
                    //     return true;
                    // int fd = dup2(out, STDOUT_FILENO);
                    // if (fd != 0)
                    //     return true;
                    // close(out);
                    exec_node_command(ast, false);
                    exit(1);
                }
                if(ast->type == TOK_LESS)
                {
                    int in = open(ast->right, O_RDONLY, 0);
                    if (in == -1)
                        return true;
                    int fd = dup2(in, STDIN_FILENO);
                    if (fd != 0)
                        return true;
                    close(in);
                    exec_node_command(ast, false);
                    exit(1);
                }
            }
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->left, ast->left);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->left);
        }
        if (ast->right && !is(ast->right, ""))
        {
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->right, ast->right);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->right);
        }
    }
    else
    {
        wait(NULL);
    }
    return false;
}
bool exec_node_prefix(struct node_prefix *ast)
{
    DEBUG("PREFIX")
    if (ast->type == ASSIGMENT_WORD)
    {
        char *key = ast->prefix.assigment_word->variable_name;
        char *val = ast->prefix.assigment_word->value;
        put_var(key, val);
    }
    if (ast->type == REDIRECTION)
        exec_node_redirection(ast->prefix.redirection);
}
bool exec_node_element(struct node_element *ast)
{
    DEBUG("ELEMENT")
    if (ast->type == WORD)
    {
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, ast->element.word);
        // if (node)
            // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    }
    if (ast->type == TOKEN_REDIRECTION)
        exec_node_redirection(ast->element.redirection);
}
bool exec_node_compound_list(struct node_compound_list *ast)
{
    DEBUG("COMPOUND_LIST")
    struct node_compound_list *c = ast;
    while (c)
    {
        if (exec_node_and_or(c->and_or))
        {
            return true;
        }
        c = c->next_sibling;
    }
    return false;
}
bool exec_node_while(struct node_while *ast)
{
    DEBUG("WHILE")
    // fprintf(f, "\tnode_%p [label=WHILE];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    exec_node_compound_list(ast->condition);
    exec_node_do_group(ast->body);

}
bool exec_node_until(struct node_until *ast)
{
    DEBUG("UNTIL")
    // fprintf(f, "\tnode_%p [label=UNTIL];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    exec_node_compound_list(ast->condition);
    exec_node_do_group(ast->body);
}
bool exec_node_case(struct node_case *ast)
{
    DEBUG("CASE")
    // fprintf(f, "\tnode_%p [label=\"CASE\n%s\"];\n", (void *)ast, ast->word);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (ast->is_case_clause)
    {
        exec_node_case_clause(ast->case_clause);
    }
}

bool exec_node_if(struct node_if *ast)
{
    DEBUG("IF")
    // fprintf(f, "\tnode_%p [label=IF];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (!exec_node_compound_list(ast->condition))
        return exec_node_compound_list(ast->if_body);
    else if (ast->else_clause)
        return exec_node_else_clause(ast->else_clause);
    return false;
}

bool exec_node_elif(struct node_if *ast)
{
    DEBUG("ELIF")
    // fprintf(f, "\tnode_%p [label=ELIF];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (!exec_node_compound_list(ast->condition))
        return exec_node_compound_list(ast->if_body);
    else if (ast->else_clause)
        return exec_node_else_clause(ast->else_clause);
    return false;
}

bool exec_node_for(struct node_for *ast)
{
    DEBUG("FOR")
    // fprintf(f, "\tnode_%p [label=FOR];\n", (void *) ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
    // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) ast->variable_name, ast->variable_name);
    // fprintf(f, "\tnode_%p -> node_%p;\n", (void *) ast, (void *) ast->variable_name);
    struct range *r = ast->range;
    struct range *tmp = (void *) ast;
    while (r)
    {
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) r, r->value);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void*) tmp, (void *) r);
        tmp = r;
        r = r->next;
    }
    exec_node_do_group(ast->body);
}
bool exec_node_else_clause(struct node_else_clause *ast)
{
    DEBUG("ELSE_CLAUSE")
    if (ast->type == ELSE)
    {
        // fprintf(f, "\tnode_%p [label=ELSE];\n", (void *) ast);
        // if (node)
            // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
        exec_node_compound_list(ast->clause.else_body);
    }
    else if (ast->type == ELIF)
    {
        exec_node_elif(ast->clause.elif);
    }
}
bool exec_node_do_group(struct node_do_group *ast)
{
    DEBUG("DO_GROUP")
    exec_node_compound_list(ast->body);
}
bool exec_node_case_clause(struct node_case_clause *ast)
{
    DEBUG("CASE_CLAUSE")
    struct node_case_clause *c = ast;
    exec_node_case_item(c->case_item);
    while (c->next)
    {
        c = c->next;
        exec_node_case_item(c->case_item);
    }
}
bool exec_node_case_item(struct node_case_item *ast)
{
    DEBUG("CASE_ITEM")
    char *s = ast->words->word;
    while (ast->words->next)
    {
        ast->words = ast->words->next;
        s = strcat(s, " | ");
        s = strcat(s, ast->words->word);
    }
    // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) ast, s);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
    if (ast->compound_list)
        exec_node_compound_list(ast->compound_list);
}
