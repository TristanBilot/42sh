#include <criterion/criterion.h>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "utils/string_utils.h"


/*                                Création de l'arbre et de ses noeuds                                    */
/*

Test(ast_node, creation_arbre_NULL){                             
    struct ast_node *node = NULL;
    cr_assert(ast_node_init(NODE_IF) == NULL);
}

Test(ast_node, creation_arbre_NULL){                                
    struct ast_node *node = ast_node_init();
    cr_assert(init_ast() != NULL);
}

Test(ast_node, test_enum_type_NODE_IF){                        
    struct ast_node *node = ast_node_if_init();
    cr_assert(node->type == NODE_IF);
}

Test(ast_node, test_enum_type_NODE_SIMPLE_COMMAND){
    struct ast_node *node = ast_node_command_init();
    cr_assert(node->type == NODE_SIMPLECOMMAND);
}

Test(ast_node, test_enum_type_NODE_WORD){
    struct ast_node *node = ast_node_word_init();
    cr_assert(node->type == NODE_WORD);
}

Test(ast_node, test_enum_type_NODE_LOGICAL){
    struct ast_node *node = ast_node_logical_init();
    cr_assert(node->type == NODE_LOGICAL);
}

Test(ast_node, test_enum_type_WRONG_NODE){
    struct ast_node *node = ast_node_logical_init();
    cr_assert(node->type == NODE_WORD);
}



*/




