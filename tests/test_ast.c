#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "utils/string.h"



/*                                Création de l'arbre et de ses noeuds                                    */


Test(ast_node, creation_arbre_NULL){                                //Test pour savoir si la création de l'arbre est NULL
    struct ast_node *tree = init_ast();
    cr_assert(init_ast() == NULL);
}

Test(ast_node, test_enum_type_NODE_IF){                        //Test pour savoir si le node est un IF
    struct ast_node *tree = init_ast_node();
    /*function parse*/
    cr_assert(tree->type == NODE_IF);
}

Test(ast_node, test_enum_type_NODE_SIMPLE_COMMAND){            // Test pour savoir si le node est une COMMAND
    struct ast_node *tree = init_ast_node();
    cr_assert(tree->type == NODE_SIMPLE_COMMAND);
}




