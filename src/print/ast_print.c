#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "./ast_print.h"
#include <err.h>
#include "../print/ast_print_dot.h"

#define PRINT_FLAG false
#define PRINT_NODE(msg) if (PRINT_FLAG) \
            fprintf(f, "%s\n", msg)

 /*digraph G {
     or -> ls;
     or -> pwd;
     itm [label=or];
     itm -> new;
 }*/

void print_node_input(struct node_input *ast, FILE *f)
{
    PRINT_NODE("PRINT INPUT");
    if (ast->node_list)
        print_node_list(ast->node_list, f);
}
void print_node_list(struct node_list *ast, FILE *f)
{
    PRINT_NODE("PRINT NODE LIST");
    struct node_list *c = ast;
    if (c->next_sibling)
    {
        if (c->type == SEMI)
        {
            fprintf(f, "\tnode_%p [label=SEMI];\n", (void *)c);
            PRINT_NODE("SEMI");
        }
        else if (c->type == SEPAND)
        {
            fprintf(f, "\tnode_%p [label=SEPAND];\n", (void *)c);
            PRINT_NODE("SEPAND");
        }
        print_node_and_or(c->and_or, f, (void *)c);
    }
    else
        print_node_and_or(c->and_or, f, NULL);
    while (c->next_sibling)
    {
        if (c->next_sibling->next_sibling)
        {
            if (c->next_sibling->type == SEMI)
            {
                fprintf(f, "\tnode_%p [label=SEMI];\n", (void *)c->next_sibling);
                fprintf(f, "\tnode_%p -> node_%p;\n", (void *) c, (void *) c->next_sibling);
                PRINT_NODE("SEMI");
            }
            else if (c->next_sibling->type == SEPAND)
            {
                fprintf(f, "\tnode_%p [label=SEPAND];\n", (void *)c->next_sibling);
                fprintf(f, "\tnode_%p -> node_%p;\n", (void *) c, (void *) c->next_sibling);
                PRINT_NODE("SEPAND");
            }
            print_node_and_or(c->next_sibling->and_or, f, (void *)c->next_sibling);
        }
        else
            print_node_and_or(c->next_sibling->and_or, f, (void *)c);
        c = c->next_sibling;
        
        // print_node_and_or(c->next_sibling->and_or, f, (void *)c);
        // c = c->next_sibling;
        // if (c->type == SEMI)
        // {
        //     fprintf(f, "\tnode_%p [label=SEMI];\n", (void *)c);
        //     PRINT_NODE("SEMI");
        // }
        // else if (c->type == SEPAND)
        // {
        //     fprintf(f, "\tnode_%p [label=SEPAND];\n", (void *)c);
        //     PRINT_NODE("SEPAND");
        // }
    }
}
void print_node_and_or(struct node_and_or *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT AND_OR");
    if (ast->right)
    {
        if (ast->type == OR)
        {
            fprintf(f, "\tnode_%p [label=OR];\n", (void *)ast);
            if (node)
                fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
            PRINT_NODE("OR");
        }
        if (ast->type == AND)
        {
            fprintf(f, "\tnode_%p [label=AND];\n", (void *)ast);
            if (node)
                fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
            
            PRINT_NODE("AND");
        }
        print_node_pipeline(ast->right, f, (void *)ast);

        if (ast->is_final)
            print_node_pipeline(ast->left.pipeline, f, (void *)ast);
        else
            print_node_and_or(ast->left.and_or, f, (void *)ast);
    }
    else
    {
        if (ast->is_final)
            print_node_pipeline(ast->left.pipeline, f, (void *)node);
        else
            print_node_and_or(ast->left.and_or, f, (void *)node);
    }
}

void print_node_pipeline(struct node_pipeline *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT PIPELINE");
    struct node_pipeline *c = ast;
    if (ast->is_not)
    {
        fprintf(f, "\tnode_%p [label=NOT];\n", (void *)ast);
        if (node)
           fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast); 
        PRINT_NODE("!");

        // c = ast;
    
        // if (c->next_sibling)
        // {
        //     fprintf(f, "\tnode_%p [label=PIPE];\n", (void *) c);
        //     fprintf(f, "\tnode_%p -> node_%p;\n", (void *)ast, (void *)c);
        //     print_node_command(c->command, f, (void *) c);
        // }
        // else
        print_node_command(c->command, f, (void*) ast);
    }
    else
    {
        if (c->next_sibling)
        {
            fprintf(f, "\tnode_%p [label=PIPE];\n", (void *) ast);
            if (node)
                fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
            print_node_command(c->command, f, (void *) ast);
        }
        else
            print_node_command(c->command, f, node);
    }
    while (c->next_sibling)
    {
        PRINT_NODE("PIPE");
        //fprintf(f, "\tnode_%p -> node_%p;\n", (void *)ast, (void *)c->next_sibling->command);
        c = c->next_sibling;
        print_node_command(c->command, f, (void *) ast);
    }
}

void print_node_command(struct node_command *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT COMMAND");
    if (ast->type == SIMPLE_COMMAND)
        print_node_simple_command(ast->command.simple_command, f, node);
    else if (ast->type == SHELL_COMMAND)
    {
        print_node_shell_command(ast->command.shell_command, f, node);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            print_node_redirection(r, f, node);
            r = r->next;
        }
    }
    else if(ast->type == FUNCDEC)
    {
        print_node_funcdec(ast->command.funcdec, f, node);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            print_node_redirection(r, f, node);
            r = r->next;
        }
    }
}
void print_node_simple_command(struct node_simple_command *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT SIMPLE COMMAND");
    struct node_prefix *p = ast->prefixes;
    struct node_element *e = ast->elements;
    if ((p && p->next) || (e && p))
    {
        fprintf(f, "\tnode_%p [label=SIMPLECOMMAND];\n", (void *)ast);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        while (p)
        {
            print_node_prefix(p, f, (void *) ast);
            p = p->next;
        }
        if (e) /* link the root node to the first element (ex: echo) */
        {
            print_node_element(e, f, (void *) ast);
            while (e->next) /* link each child between them */
            {
                print_node_element(e->next, f, e);
                e = e->next;
            }
        }
    }
    else
    {
        while (p)
        {
            print_node_prefix(p, f, node);
            p = p->next;
        }
        if (e) /* link the root node to the first element (ex: echo) */
        {
            print_node_element(e, f, node);
            while (e->next) /* link each child between them */
            {
                print_node_element(e->next, f, e);
                e = e->next;
            }
        }
    }
    
    
}
/* NE PAS OUBLIER D'IMPLEMENTER LES TYPES POUR UNIONS  */
void print_node_shell_command(struct node_shell_command *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT SHELL COMMAND");
    switch (ast->type)
    {
    case C_BRACKETS:
        PRINT_NODE("{");
        fprintf(f, "\tnode_%p [label=C_BRACKET];\n", (void *)ast);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        print_node_compound_list(ast->shell.compound_list, f, ast);
        PRINT_NODE("}");
        break;
    case PARENTHESIS:
        PRINT_NODE("(");
        fprintf(f, "\tnode_%p [label=PARENTHESIS];\n", (void *)ast);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        print_node_compound_list(ast->shell.compound_list, f, ast);
        PRINT_NODE(")");
        //fprintf(f, "\tnode_%p [label=C_PARENTHESIS_CLOSE];\n", (void *)ast);
        break;
    case RULE:
        if (ast->shell_type == WHILE)
            print_node_while(ast->shell.rule_while, f, node);
        if (ast->shell_type == FOR)
        {
            printf("enter for\n");
            print_node_for(ast->shell.rule_for, f, node);
        }
        if (ast->shell_type == UNTIL)
            print_node_until(ast->shell.rule_until, f, node);
        if (ast->shell_type == CASE)
            print_node_case(ast->shell.rule_case, f, node);
        if (ast->shell_type == IF)
            print_node_if(ast->shell.rule_if, f, node);
        break;
    default:
        break;
    }
}
void print_node_funcdec(struct node_funcdec *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT FUNCDEC");
    if (ast->is_function)
    {
        fprintf(f, "\tnode_%p [label=FUNCTION];\n", (void *)ast);
        PRINT_NODE("function");
    }
    else
        fprintf(f, "\tnode_%p [label=FUNCTION_CALL];\n", (void *)ast);
    fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        
    fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->function_name, ast->function_name);
    fprintf(f, "\tnode_%p -> node_%p;\n", (void *)ast, (void *)ast->function_name);
    PRINT_NODE(ast->function_name);
    PRINT_NODE("(");
    PRINT_NODE(")");
    print_node_shell_command(ast->shell_command, f, (void *)ast);
}

void print_node_redirection(struct node_redirection *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT REDIRECTION");
    char *redirection = type_to_str(ast->type);
    fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, redirection);
    fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (ast->left && !is(ast->left, ""))
    {
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->left, ast->left);
        fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->left);
        PRINT_NODE(ast->left);
    }
    if (ast->right && !is(ast->right, ""))
    {
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast->right, ast->right);
        fprintf(f, "\tnode_%p -> node_%p;\n", (void*)ast, (void *)ast->right);
        PRINT_NODE(ast->right);
    }
    
}
void print_node_prefix(struct node_prefix *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT PREFIX");
    
    if (ast->type == ASSIGMENT_WORD)
    {
        char *s = strcat(ast->prefix.assigment_word->variable_name, "=");
        char *s1 = strcat(s, ast->prefix.assigment_word->value);
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, s1);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
        PRINT_NODE(ast->prefix.assigment_word->variable_name);
        PRINT_NODE("=");
        PRINT_NODE(ast->prefix.assigment_word->value);
        PRINT_NODE("");
    }
    if (ast->type == REDIRECTION)
        print_node_redirection(ast->prefix.redirection, f, node);
}
void print_node_element(struct node_element *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT ELEMENT");
    if (ast->type == WORD)
    {
        PRINT_NODE(ast->element.word);
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *)ast, ast->element.word);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    }
    if (ast->type == TOKEN_REDIRECTION)
        print_node_redirection(ast->element.redirection, f, node);
}
void print_node_compound_list(struct node_compound_list *ast, FILE *f, void *node)
{

    PRINT_NODE("PRINT COMPOUND LIST");
    struct node_compound_list *c = ast;
    while (c)
    {
        print_node_and_or(c->and_or, f, node);
        c = c->next_sibling;
    }
}
void print_node_while(struct node_while *ast, FILE *f, void *node)
{   //while a do echo toto done
    PRINT_NODE("PRINT WHILE");
    fprintf(f, "\tnode_%p [label=WHILE];\n", (void *)ast);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    print_node_compound_list(ast->condition, f, (void*)ast);
    print_node_do_group(ast->body, f, (void*)ast);

}
void print_node_until(struct node_until *ast, FILE *f, void *node)
{
     PRINT_NODE("PRINT UNTIL");
    fprintf(f, "\tnode_%p [label=UNTIL];\n", (void *)ast);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    print_node_compound_list(ast->condition, f, (void*)ast);
    print_node_do_group(ast->body, f, (void*)ast);
}
void print_node_case(struct node_case *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINTCASE");
    //printf("%s\n", ast->word);
    PRINT_NODE("IN");
    fprintf(f, "\tnode_%p [label=\"CASE\n%s\"];\n", (void *)ast, ast->word);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    if (ast->is_case_clause)
    {
        //PRINT_NODE("CASE CLAUSE");
        print_node_case_clause(ast->case_clause, f, (void *)ast);
    }
    PRINT_NODE("ESAC");
}

void print_node_if(struct node_if *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT IF");
    fprintf(f, "\tnode_%p [label=IF];\n", (void *)ast);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    print_node_compound_list(ast->condition, f, (void*)ast);
    PRINT_NODE("THEN");
    print_node_compound_list(ast->if_body, f, (void*)ast);
    if (ast->else_clause)
    {
        //PRINT_NODE("ELSE");
        // fprintf(f, "\tnode_%p [label=ELSE];\n", (void *) ast->else_clause);
        // fprintf(f, "\tnode_%p -> node_%p;\n", (void *)ast, (void *) ast->else_clause);
        print_node_else_clause(ast->else_clause, f, (void*)ast);
    }
    PRINT_NODE("FI");
}

void print_node_elif(struct node_if *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT ELIF");
    fprintf(f, "\tnode_%p [label=ELIF];\n", (void *)ast);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *)ast);
    print_node_compound_list(ast->condition, f, (void*)ast);
    PRINT_NODE("THEN");
    print_node_compound_list(ast->if_body, f, (void*)ast);
    if (ast->else_clause)
    {
        print_node_else_clause(ast->else_clause, f, (void*)ast);
    }
}

void print_node_for(struct node_for *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT FOR");
    PRINT_NODE(ast->variable_name);
    PRINT_NODE("IN");
    // trouver le moyen de gerer les ;;
    fprintf(f, "\tnode_%p [label=FOR];\n", (void *) ast);
    if (node)
        fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
    fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) ast->variable_name, ast->variable_name);
    fprintf(f, "\tnode_%p -> node_%p;\n", (void *) ast, (void *) ast->variable_name);
    struct range *r = ast->range;
    struct range *tmp = (void *) ast;
    while (r)
    {
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) r, r->value);
        fprintf(f, "\tnode_%p -> node_%p;\n", (void*) tmp, (void *) r);
        tmp = r;
        PRINT_NODE(r->value);
        r = r->next;
    }
    print_node_do_group(ast->body, f, (void *) ast);
}
void print_node_else_clause(struct node_else_clause *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT ELSE CLAUSE");
    if (ast->type == ELSE)
    {
        PRINT_NODE("ELSE");
        fprintf(f, "\tnode_%p [label=ELSE];\n", (void *) ast);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
        print_node_compound_list(ast->clause.else_body, f, (void *) ast);
    }
    else if (ast->type == ELIF)
    {
        PRINT_NODE("ELIF");
        print_node_elif(ast->clause.elif, f, node);
    }
}
void print_node_do_group(struct node_do_group *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT DO GROUP");
    print_node_compound_list(ast->body, f, node);
    PRINT_NODE("DONE");
}
void print_node_case_clause(struct node_case_clause *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT CASE CLAUSE");
    struct node_case_clause *c = ast;
    print_node_case_item(c->case_item, f, node);
    while (c->next)
    {
        PRINT_NODE(";;");
        c = c->next;
        print_node_case_item(c->case_item, f, node);
    }
    PRINT_NODE(";;");
}
void print_node_case_item(struct node_case_item *ast, FILE *f, void *node)
{
    PRINT_NODE("PRINT CASE ITEM");
    PRINT_NODE(ast->words->word);
    if (ast && ast->words)
    {
        char *s = ast->words->word;
        while (ast->words->next)
        {
            ast->words = ast->words->next;
            s = strcat(s, " | ");
            s = strcat(s, ast->words->word);
            PRINT_NODE(ast->words->word);
        }
        fprintf(f, "\tnode_%p [label=\"%s\"];\n", (void *) ast, s);
        if (node)
            fprintf(f, "\tnode_%p -> node_%p;\n", node, (void *) ast);
        if (ast->compound_list)
            print_node_compound_list(ast->compound_list, f, (void *) ast);
    }

}

void print_ast(struct node_input *ast)
{
    if (!ast)
        return;
    // print_node_input(ast, stdout);
    FILE *f = new_dot();
    print_node_input(ast, f);
    close_dot(f);
    convert_dot_to_png();
}
