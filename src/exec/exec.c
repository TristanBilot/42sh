#include "exec.h"
#include "utils/string.h"
#include <fcntl.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define STDOUT_FILENO 1       /* Standard output.  */

bool execute(char **args)
{
    // int	status = 0;
    // int	child = 0;
    // if ((child = fork()) == -1)
    //     return true;
    // if (child == 0)
    // {
        // if (!should_print)
        //     strcat(args[0], " >/dev/null");
        if ((execvp(args[0], args)) == -1)
        {
            err(1, "command not found: %s\n", args[0]);
            return true;
        }
    // close(fd[READ_END]);
    // close(fd[WRITE_END]);
        // return false;
    // }
    // else
    // {
    //     wait(&status);
    //     if (WIFEXITED(status))
    //     {
    //         printf("exit status = %d\n", WEXITSTATUS(status));
    //         close(fd[READ_END]);
    //         close(fd[WRITE_END]);
    //         return WEXITSTATUS(status) == 1; /* 1 = no output in stdout */
    //     }
    // }
    // close(fd[READ_END]);
    // close(fd[WRITE_END]);
    return false;
}

bool exec_node_input(struct node_input *ast)
{
    return exec_node_list(ast->node_list);
}
bool exec_node_list(struct node_list *ast)
{
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
    struct node_pipeline *c = ast;
    bool state = false;

    if (!c->next_sibling)
    {
        if (ast->is_not)
            return !exec_node_command(c->command);
        return exec_node_command(c->command);
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
                exit(exec_node_command(c->command));
            }
        }
    }
    return false;
}

bool exec_node_command(struct node_command *ast)
{
    bool state = false;
    if (ast->type == SIMPLE_COMMAND)
        return exec_node_simple_command(ast->command.simple_command);
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
bool exec_node_simple_command(struct node_simple_command *ast)
{
    struct node_prefix *p = ast->prefixes;
    struct node_element *e = ast->elements;
    // fprintf(f, "\tnode_%p [label=SIMPLECOMMAND];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    while (p)
    {
        exec_node_prefix(p);
        p = p->next;
    }
    if (e) /* link the root node to the first element (ex: echo) */
    {
        char *args[256];
        int size = 0;
        args[size++] = e->element.word;
        while (e->next) /* link each child between them */
        {
            e = e->next;
            args[size++] = e->element.word;
        }
        args[size++] = NULL;
        return execute(args);
    }
}

bool exec_node_shell_command(struct node_shell_command *ast)
{
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
    char *redirection = type_to_str(ast->type);
    // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, redirection);
    // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (ast->left && !is(ast->left, ""))
    {
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->left, ast->left);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->left);
    }
    if (ast->right && !is(ast->right, ""))
    {
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->right, ast->right);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->right);
    }
}
bool exec_node_prefix(struct node_prefix *ast)
{
    if (ast->type == ASSIGMENT_WORD)
    {
        char *s = strcat(ast->prefix.assigment_word->variable_name, "=");
        char *s1 = strcat(s, ast->prefix.assigment_word->value);
        // fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, s1);
        // if (node)
            // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    }
    if (ast->type == REDIRECTION)
        exec_node_redirection(ast->prefix.redirection);
}
bool exec_node_element(struct node_element *ast)
{
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
    // fprintf(f, "\tnode_%p [label=WHILE];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    exec_node_compound_list(ast->condition);
    exec_node_do_group(ast->body);

}
bool exec_node_until(struct node_until *ast)
{
    // fprintf(f, "\tnode_%p [label=UNTIL];\n", (void *)ast);
    // if (node)
        // fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    exec_node_compound_list(ast->condition);
    exec_node_do_group(ast->body);
}
bool exec_node_case(struct node_case *ast)
{
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
    printf("NODE_IF");
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
    exec_node_compound_list(ast->body);
}
bool exec_node_case_clause(struct node_case_clause *ast)
{
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
