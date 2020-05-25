#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "./arithmetic_lexer.h"
#include "../../../utils/xalloc.h"
#include "../../../utils/string_utils.h"
#include "../../../utils/buffer.h"
#include "../../../utils/bracket_counter.h"

bool is_lexer_valid(struct arithmetic_lexer *lexer)
{
    struct arithmetic_token *curr = lexer->token_list->first;
    if (curr && curr->type != TOK_A_NUMBER && curr->type != TOK_A_END
        && curr->type != TOK_A_MINUS && curr->type != TOK_A_PLUS
        && curr->type != TOK_A_LPAR && curr->type != TOK_A_NOT
        && curr->type != TOK_A_TILDE)
        return false;
    while (curr && curr->next)
    {
        if  (!is_valid_arithmetic_syntax(curr->type, curr->next->type))
        {
            fprintf(stderr, "err: %s %s\n", token_str(curr->type),
                token_str(curr->next->type));
            return false;
        }
        curr = curr->next;
    }
    return true;
}

bool init_arithmetic_lexer(struct arithmetic_lexer *lexer)
{
    char c;
    int type;
    struct buffer *buf = new_buffer();

    if (count_closed_occurences(lexer->input, 0, COUNT_PAREN))
        return false;

    size_t len = strlen(lexer->input) - 1;
    while (lexer->input[len] == ' ')
        len--;
    len++;

    for (size_t i = 0; i < len; i++)
    {
        while (i < len && (c = lexer->input[i]) == ' ')
            i++;
        if (i >= len)
            break;
        if ((type = eval_arithmetic_char(lexer->input, i)) == TOK_A_UNKNOWN)
        {
            return false;
        }
        if (type == TOK_A_OR || type == TOK_A_AND || type == TOK_A_POW)
            i++;

        bool last = (i == len - 1);
        if (type != TOK_A_NUMBER || last)
        {
            if (type == TOK_A_NUMBER && last)
            {
                append_buffer(buf, c);
                append_arithmetic(lexer,
                    new_arithmetic_number_token(atoi(buf->buf)));
            }
            else if (buffer_len(buf) > 0)
            {
                append_arithmetic(lexer,
                    new_arithmetic_number_token(atoi(buf->buf)));
                flush(buf);
            }
            if (type != TOK_A_NUMBER)
                append_arithmetic(lexer, new_arithmetic_token(type));
        }
        else
            append_buffer(buf, c);
    }
    append_arithmetic(lexer, new_arithmetic_token(TOK_A_END));
    return is_lexer_valid(lexer);
}

struct arithmetic_lexer *new_arithmetic_lexer(char *str)
{
    struct arithmetic_lexer *lexer = xmalloc(sizeof(struct arithmetic_lexer));
    lexer->token_list = xmalloc(sizeof(struct arithmetic_token_list));
    lexer->token_list->first = NULL;
    lexer->token_list->next = NULL;
    lexer->token_list->last = NULL;
    lexer->input = str;
    bool state = init_arithmetic_lexer(lexer);
    return state ? lexer : NULL;
}


struct arithmetic_token *peek_arithmetic(struct arithmetic_lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    return lexer->token_list->next;
}

struct arithmetic_token *pop_arithmetic(struct arithmetic_lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    struct arithmetic_token *next = lexer->token_list->next;
    lexer->token_list->next = next->next;
    return next;
}

void append_arithmetic(struct arithmetic_lexer *lexer,
    struct arithmetic_token *token)
{
    if (!lexer || !token)
        return;
    if (!lexer->token_list->last)
    {
        lexer->token_list->next = token;
        lexer->token_list->last = token;
        lexer->token_list->first = token;
        lexer->token_list->next->next = NULL;
        return;
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
