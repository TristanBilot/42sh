#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "eval/ast_print.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "print/ast_print_dot.h"

#define PRINT_FLAG true
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
    print_node_list(ast->node_list, f);
}
void print_node_list(struct node_list *ast, FILE *f)
{
    PRINT_NODE("PRINT NODE LIST");
    // printf("====> %p\n", ast->and_or->left.pipeline->command->command.simple_command->prefixes);
    struct node_list *c = ast;
    print_node_and_or(c->and_or, f);
    while (c->next_sibling)
    {
        c = c->next_sibling;
        if (c->type == SEMI)
        {
            PRINT_NODE("SEMI");
        }
        else if (c->type == SEPAND)
        {
            PRINT_NODE("SEPAND");
        }
        print_node_and_or(c->and_or, f);
    }
}
void print_node_and_or(struct node_and_or *ast, FILE *f)
{
    PRINT_NODE("PRINT AND_OR");
    if (ast->right)
    {
        if (ast->type == OR)
        {
            PRINT_NODE("OR");
        }
        if (ast->type == AND)
            PRINT_NODE("AND");
        print_node_pipeline(ast->right, f);
        //char *arr[] = { str(ast), " [label=or]" };
        //printf("===> %s", concat(arr));
        //append_to_dot(f, concat(arr), false);
    }

    if (ast->is_final)
        print_node_pipeline(ast->left.pipeline, f);
    else
        print_node_and_or(ast->left.and_or, f);
    
}

void print_node_pipeline(struct node_pipeline *ast, FILE *f)
{
    PRINT_NODE("PRINT PIPELINE");
    //printf("is_not = %d\n", ast->is_not);
    if (ast->is_not)
        PRINT_NODE("!");
    
    struct node_pipeline *c = ast;
    print_node_command(c->command, f);
    while (c->next_sibling)
    {
        PRINT_NODE("PIPE");
        c = c->next_sibling;
        print_node_command(c->command, f);
    }
}

void print_node_command(struct node_command *ast, FILE *f)
{
    PRINT_NODE("PRINT COMMAND");
    if (ast->type == SIMPLE_COMMAND)
        print_node_simple_command(ast->command.simple_command, f);
    else if (ast->type == SHELL_COMMAND)
    {
        print_node_shell_command(ast->command.shell_command, f);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            print_node_redirection(r, f);
            r = r->next;
        }
    }
    else if(ast->type == FUNCDEC)
    {
        print_node_funcdec(ast->command.funcdec, f);
        struct node_redirection *r = ast->redirections;
        while(r)
        {
            print_node_redirection(r, f);
            r = r->next;
        }
    }
}
void print_node_simple_command(struct node_simple_command *ast, FILE *f)
{
    PRINT_NODE("PRINT SIMPLE COMMAND");
    struct node_prefix *p = ast->prefixes;
    struct node_element *e = ast->elements;
    while (p)
    {
        print_node_prefix(p, f);
        p = p->next;
    }
    while (e)
    {
        print_node_element(e, f);
        e = e->next;
    }
}
/* NE PAS OUBLIER D'IMPLEMENTER LES TYPES POUR UNIONS  */
void print_node_shell_command(struct node_shell_command *ast, FILE *f)
{
    PRINT_NODE("PRINT SHELL COMMAND");
    switch (ast->type)
    {
    case C_BRACKETS:
        PRINT_NODE("{");
        print_node_compound_list(ast->shell.compound_list, f);
        PRINT_NODE("}");
        break;
    case PARENTHESIS:
        PRINT_NODE("(");
        print_node_compound_list(ast->shell.compound_list, f);
        PRINT_NODE(")");
        break;
    case RULE:
        if (ast->shell_type == WHILE)
            print_node_while(ast->shell.rule_while, f);
        if (ast->shell_type == FOR)
        {
            printf("enter for\n");
            print_node_for(ast->shell.rule_for, f);
        }
        if (ast->shell_type == UNTIL)
            print_node_until(ast->shell.rule_until, f);
        if (ast->shell_type == CASE)
            print_node_case(ast->shell.rule_case, f);
        if (ast->shell_type == IF)
            print_node_if(ast->shell.rule_if, f);
        break;
    default:
        break;
    }
}
void print_node_funcdec(struct node_funcdec *ast, FILE *f)
{
    PRINT_NODE("PRINT FUNCDEC");   
    if (ast->is_function)
        PRINT_NODE("function");
    PRINT_NODE(ast->function_name);
    PRINT_NODE("(");
    PRINT_NODE(")");
    print_node_shell_command(ast->shell_command, f);
}


void print_node_redirection(struct node_redirection *ast, FILE *f)
{
    PRINT_NODE("PRINT REDIRECTION");
    if (ast->left)
    {
        printf("a\n");
        PRINT_NODE(ast->left);
    }
    if (TOK_DLESSDASH == ast->type)
        PRINT_NODE("<<-");
    if (TOK_DLESS == ast->type)
        PRINT_NODE("<<");
    if (TOK_LESSGREAT == ast->type)
        PRINT_NODE("<>");
    if (TOK_LESS == ast->type)
        PRINT_NODE("<");
    if (TOK_DGREAT == ast->type)
        PRINT_NODE(">>");
    if (TOK_GREATAND == ast->type)
        PRINT_NODE(">&");
    if (TOK_CLOBBER == ast->type)
        PRINT_NODE(">|");
    if (TOK_GREAT == ast->type)
        PRINT_NODE(">");

    PRINT_NODE(ast->right);
}
void print_node_prefix(struct node_prefix *ast, FILE *f)
{
    PRINT_NODE("PRINT PREFIX");
    if (ast->type == ASSIGMENT_WORD)
    {
        PRINT_NODE(ast->prefix.assigment_word->variable_name);
        PRINT_NODE("=");
        PRINT_NODE(ast->prefix.assigment_word->value);
        PRINT_NODE("");
    }
    if (ast->type == REDIRECTION)
        print_node_redirection(ast->prefix.redirection, f);
}
void print_node_element(struct node_element *ast, FILE *f)
{
    PRINT_NODE("PRINT ELEMENT");
    if (ast->type == WORD)
        PRINT_NODE(ast->element.word);
    if (ast->type == TOKEN_REDIRECTION)
        print_node_redirection(ast->element.redirection, f);
}
void print_node_compound_list(struct node_compound_list *ast, FILE *f)
{
    PRINT_NODE("PRINT COMPOUND LIST");
    struct node_compound_list *c = ast;
    while (c)
    {
        print_node_and_or(c->and_or, f);
        c = c->next_sibling;
    }
}
void print_node_while(struct node_while *ast, FILE *f)
{
    PRINT_NODE("PRINT WHILE");
    print_node_compound_list(ast->condition, f);
    print_node_do_group(ast->body, f);
}
void print_node_until(struct node_until *ast, FILE *f)
{
    PRINT_NODE("PRINT UNTIL");
    print_node_compound_list(ast->condition, f);
    print_node_do_group(ast->body, f);
}
void print_node_case(struct node_case *ast, FILE *f)
{
    PRINT_NODE("PRINTCASE");
    //printf("%s\n", ast->word);
    PRINT_NODE("IN");
    if (ast->is_case_clause)
    {
        //PRINT_NODE("CASE CLAUSE");
        print_node_case_clause(ast->case_clause, f);
    }
    PRINT_NODE("ESAC");
}

void print_node_if(struct node_if *ast, FILE *f)
{
    PRINT_NODE("PRINT IF");
    print_node_compound_list(ast->condition, f);
    PRINT_NODE("THEN");
    print_node_compound_list(ast->if_body, f);
    if (ast->else_clause)
    {
        PRINT_NODE("ELSE");
        print_node_else_clause(ast->else_clause, f);
    }
    PRINT_NODE("FI");
}

void print_node_for(struct node_for *ast, FILE *f)
{
    PRINT_NODE("PRINT FOR");
    PRINT_NODE(ast->variable_name);
    PRINT_NODE("IN");
    // trouver le moyen de gerer les ;;                             
    struct range *r = ast->range;
    while (r)
    {
        PRINT_NODE(r->value);
        r = r->next;
    }
    print_node_do_group(ast->body, f);
}
void print_node_else_clause(struct node_else_clause *ast, FILE *f)
{
    PRINT_NODE("PRINT ELSE CLAUSE");
    if (ast->type == ELSE)
    {
        PRINT_NODE("ELSE");
        print_node_compound_list(ast->clause.else_body, f);
    }
    else if (ast->type == ELIF)
    {
        PRINT_NODE("ELIF");
        print_node_if(ast->clause.elif, f);
    }
}
void print_node_do_group(struct node_do_group *ast, FILE *f)
{
    PRINT_NODE("PRINT DO GROUP");
    print_node_compound_list(ast->body, f);
    PRINT_NODE("DONE");
}
void print_node_case_clause(struct node_case_clause *ast, FILE *f)
{
    PRINT_NODE("PRINT CASE CLAUSE");
    struct node_case_clause *c = ast;
    print_node_case_item(c->case_item, f);
    while (c->next)
    {
        PRINT_NODE(";;");
        c = c->next;
        print_node_case_item(c->case_item, f);
    }
    PRINT_NODE(";;");
}
void print_node_case_item(struct node_case_item *ast, FILE *f)
{
    PRINT_NODE("PRINT CASE ITEM");
    PRINT_NODE(ast->words->word);
    while (ast->words->next)
    {
        ast->words = ast->words->next;
        PRINT_NODE(ast->words->word);
    }
    if (ast->compound_list)
        print_node_compound_list(ast->compound_list, f);

}

static void print_ast(struct node_input *ast)
{
    if (!ast)
        return;
    print_node_input(ast, stdout);
    // FILE *f = new_dot();
    // print_node_input(ast, f);
    // close_dot(f);
    // convert_dot_to_png();
}


// void tree_traversal(struct ast_node *node)
// {
//     print_node(node);
//     for (size_t index = 0; index < nb_children; index++)
//         tree_traversal(node->children[index]);
// }



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Usage: ./test \"expression\"");
        return 1;
    }

    struct lexer *lexer = new_lexer(argv[1]);
    struct node_input *ast = NULL;

    if ((ast = parse(lexer)))
    {
        print_ast(ast);
    }
    // else
    //     puts("Parsing error");

    free_lexer(lexer);
}
