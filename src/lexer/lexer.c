#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/xalloc.h"
#include "lexer/token.h"
#include "lexer/lexer.h"

#define MAX_STR_LEN 256

int is(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

enum token_type evaluate_token(char *c)
{
    if (is(c, "\n"))
        return TOK_NEWLINE;
    else if (is(c, "&&"))
        return TOK_AND;
    else if (is(c, "&"))
        return TOK_SEPAND;
    else if (is(c, "||"))
        return TOK_OR;
    else if (is(c, "|"))
        return TOK_PIPE;
    else if (is(c, ";;"))
        return TOK_DSEMI;
    else if (is(c, ";"))
        return TOK_SEMI;
    else if (is(c, "("))
        return TOK_LPAREN;
    else if (is(c, ")"))
        return TOK_RPAREN;
    else if (is(c, "<<-"))
        return TOK_DLESSDASH;
    else if (is(c, "<<"))
        return TOK_DLESS;
    else if (is(c, "<>"))
        return TOK_LESSGREAT;
    else if (is(c, "<&"))
        return TOK_LESSAND;
    else if (is(c, "<"))
        return TOK_LESS;
    else if (is(c, ">>"))
        return TOK_DGREAT;
    else if (is(c, ">&"))
        return TOK_GREATAND;
    else if (is(c, ">|"))
        return TOK_CLOBBER;
    else if (is(c, ">"))
        return TOK_GREAT;
    return TOK_WORD;
}

char **split(const char *str)
{
    char delim[] = " ";
    char **res = calloc(1, sizeof(char *) * MAX_TOKEN);
    char *splitted = strtok(strdup(str), delim);
    int i = 0;
    while (splitted != NULL)
    {
        res[i] = xmalloc(sizeof(char *));
        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    return res;
}

int evaluate_number(char *str, size_t *index)
{
    int res = 0;
    while (index && *index < strlen(str) &&
        str[*index] >= '0' && str[*index] <= '9')
    {
        res *= 10;
        res += (str[(*index)++] - '0');
    }
    (*index)--;
    return res;
}

void flush(char *buffer, int size)
{
    for (int i = 0; i < size; i++)
        buffer[i] = '\0';
}

void lexer_init(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    char *c;
    int i = 0;
    int type;
    char buffer[MAX_STR_LEN];
    while (splitted[i] != '\0')
    {
        c = splitted[i];
        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                char buf[2];
                buf[0] = c[j];
                buf[1] = '\0';
                strcat(buffer, buf);
                if ((type = evaluate_token(buffer)) != TOK_WORD && 
                    c[j+1] && (c[j+1] == ' ' || c[j+1] == '\n'))
                {
                    append(lexer, new_token_type(type));
                    flush(buffer, MAX_STR_LEN);
                }
                if (j == strlen(c) - 1)
                {
                    append(lexer, new_token_word(c));
                    flush(buffer, MAX_STR_LEN);
                }
            }
        }
        else
            append(lexer, new_token_type(type));
        i++;
    }
    append(lexer, new_token());
}

struct lexer *new_lexer(const char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
    lexer->input = str;
    lexer_init(lexer);
    return lexer;
}

void free_lexer(struct lexer *lexer)
{
    if (!lexer)
        return;
    struct token *index = lexer->token_list->first;
    struct token *tmp = NULL;
    while (index && index->next)
    {
        tmp = index;
        free(tmp);
        index = index->next;
    }
    free(lexer);
}

struct token *peek(struct lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next)
        return NULL;
    return lexer->token_list->next;
}

struct token *pop(struct lexer *lexer)
{
    if (!lexer->token_list ||
        !lexer->token_list->next ||
        !lexer->token_list->next->next)
        return NULL;
    struct token *next = lexer->token_list->next;
    lexer->token_list->next = next->next;
    return next;
}

void append(struct lexer *lexer, struct token *token)
{
    if (!lexer || !token)
        return;
    if (!lexer->token_list->last)
    {
        lexer->token_list->next = token;
        lexer->token_list->last = token;
        lexer->token_list->first = token;
        lexer->token_list->next->next = NULL;
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
