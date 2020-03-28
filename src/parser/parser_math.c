/* /!\ forward function declaration /!\
**
** This solves a cyclique dependency between the 3 parsing functions
**
**      exp ----> sexp ----> texp +
**       ^                        |
**       + ---------------------- +
**
** this means at least one function must be declares before the 3 are
** defined.
*/
static bool parse_exp(struct lexer *lexer, struct ast **ast);

static bool parse_parenthesis(struct lexer *lexer, struct ast **ast)
{
    bool res = parse_exp(lexer, ast);

    struct token *token = lexer_pop(lexer);
    res = res && token && token->type == TOKEN_RIGHT_PARENTHESIS;

    token_free(token);
    return res;
}

static bool parse_texp(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_pop(lexer);
    if (!token)
        return false;

    bool res = true;
    if (token->type == TOKEN_LEFT_PARENTHESIS)
        res = parse_parenthesis(lexer, ast);
    else if (token->type == TOKEN_NUMBER)
        *ast  = ast_alloc_number(token->value);
    else if (token->type == TOKEN_MINUS)
    {
        token_free(token);
        token = lexer_pop(lexer);

        if (token && token->type == TOKEN_NUMBER)
            *ast = ast_alloc_number(-token->value);
        else
            res = false;
    }
    else
        res = false;

    token_free(token);
    return res;
}

static bool parse_sexp(struct lexer *lexer, struct ast **ast)
{
    if (!parse_texp(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek(lexer);
    while (res && token && token_is_sop(token))
    {
        token = lexer_pop(lexer);

        struct ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOKEN_MULTIPLY:
            tmp->type = EXPR_MULTIPLICATION;
            break;
        case TOKEN_DIVIDE:
            tmp->type = EXPR_DIVISION;
            break;
        default:
            res = false;
            break;
        }
        res = parse_texp(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek(lexer);
    };

    return res;
}

static bool parse_exp(struct lexer *lexer, struct ast **ast)
{
    if (!parse_sexp(lexer, ast))
        return false;

    bool res = true;
    struct token *token = lexer_peek(lexer);
    while (res && token && token_is_eop(token))
    {
        token = lexer_pop(lexer);

        struct ast *tmp = ast_alloc();
        switch (token->type)
        {
        case TOKEN_PLUS:
            tmp->type = EXPR_ADDITION;
            break;
        case TOKEN_MINUS:
            tmp->type = EXPR_SUBTRACTION;
            break;
        default:
            res = false;
            break;
        }
        res = parse_sexp(lexer, &tmp->data.children.right);
        tmp->data.children.left = *ast;
        *ast = tmp;
        token_free(token);
        token = lexer_peek(lexer);
    };

    return res;
}

bool parse_expression(struct lexer *lexer, struct ast **ast)
{
    struct token *token = lexer_peek(lexer);
    if (!token || token->type == TOKEN_EOF)
        return false;

    if (!parse_exp(lexer, ast))
        return false;

    token = lexer_peek(lexer);
    return token && token->type == TOKEN_EOF;
}
