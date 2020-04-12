#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "eval/ast_print.h"
#include <err.h>
#include <stdio.h>


// FILE* fopen(const char* nomDuFichier, const char* modeOuverture);    

void print_node_input(struct node_input *ast, FILE *f)
{
    
}
void print_node_list(struct node_list *ast, FILE *f);
void print_node_and_or(struct node_and_or *ast, FILE *f);
void print_node_pipeline(struct node_pipeline *ast, FILE *f);
void print_node_command(struct node_command *ast, FILE *f);
void print_node_simple_command(struct node_simple_command *ast, FILE *f);
void print_node_shell_command(struct node_shell_command *ast, FILE *f);
void print_node_funcdec(struct node_funcdec *ast, FILE *f);
void print_node_redirection(struct node_redirection *ast, FILE *f);
void print_node_prefix(struct node_prefix *ast, FILE *f);
void print_node_element(struct node_element *ast, FILE *f);
void print_node_compound_list(struct node_compound_list *ast, FILE *f);
void print_node_while(struct node_while *ast, FILE *f);
void print_node_until(struct node_until *ast, FILE *f);
void print_node_case(struct node_case *ast, FILE *f);
void print_node_if(struct node_if *ast, FILE *f);
void print_node_for(struct node_for *ast, FILE *f);
void print_node_else_clause(struct node_else_clause *ast, FILE *f);
void print_node_do_group(struct node_do_group *ast, FILE *f);
void print_node_case_clause(struct node_case_clause *ast, FILE *f);
void print_node_case_item(struct node_case_item *ast, FILE *f);

static void print_ast(struct node_input *ast)
{
    if (!ast)
        return;
    
}


void tree_traversal(struct ast_node *node)
{
    print_node(node);
    for (size_t index = 0; index < nb_children; index++)
        tree_traversal(node->children[index]);
}



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
        print_ast(ast);
    // else
    //     puts("Parsing error\n");

    free_lexer(lexer);
}
