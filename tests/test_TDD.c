#include <criterion/criterion.h>



//Définition d'un test
Test(suite_name, test_name) {
    // test contents
}

//Modèle de test critérion
Test(sample, test) {
    cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
    cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
    cr_assert(strlen("") == 0);
}


Test(AST, creation_arbre_NULL){                                //Test pour savoir si la création de l'arbre est NULL
    struct AST *tree = init_ast();
    cr_assert(init_ast() == NULL);
}

Test(ast_node, test_enum_type_NODE_IF){                        //Test pour savoir si le node est un IF
    struct ast_node *tree = init_ast_node();
    struct token *token = malloc(sizeof(struct token));
    token->type = 
    /*function parse*/
    cr_assert(tree->type == NODE_IF);
}

Test(ast_node, test_enum_type_NODE_SIMPLE_COMMAND){            // Test pour savoir si le node est une COMMAND
    struct ast_node *tree = init_ast_node();
    cr_assert(tree->type == NODE_SIMPLE_COMMAND);
}


