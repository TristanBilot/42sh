#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/xalloc.h"
#include "../utils/string_utils.h"
#include "../utils/buffer.h"
#include "../lexer/token.h"
#include "../lexer/lexer.h"
#include "../lexer/lex_evaluation.h"
#include "../utils/string_utils.h"
#include "../utils/index_utils.h"
#include "../utils/bracket_counter.h"
#include "../storage/program_data_storage.h"

bool is_word = false;
bool is_kw_in = false;
bool is_ass_w = false;

char **split(char *str)
{
    if (!str)
        return NULL;
    const char delim[2] = " ";
    char **res = xcalloc(1, sizeof(char *) * MAX_TOKEN);
    char *splitted = strtok(my_strdup(str), delim);
    int i = 0;
    while (splitted != NULL)
    {
        res[i++] = splitted;
        splitted = strtok(NULL, delim);
    }
    res[i] = NULL;
    free(splitted);
    return res;
}

int lex_full(struct lexer *lexer, char *c, size_t j)
{
    char *value = NULL;
    struct token *token = NULL;
    if ((value = lex_backslash(c, j)))
        append(lexer, new_token_word(value));
    return (token || value) ? 1 : 0;
}

/*
* return 1: break
* return -1: continue
* return 0: nothing
*/
int lex_parenthesis(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    int type;
    if (c[*j] == ')')
    {
        if (*j == 0) /* )if */
        {
            append(lexer, new_token_type(TOK_RPAREN));
            flush(buffer);
            return -1;
        }
        char *content = substr(c, 0, *j);
        if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
        {
            append(lexer, new_token_type(type));
            append(lexer, new_token_type(TOK_RPAREN));
            if (c[*j + 1])
            {
                flush(buffer);
                return -1;
            }
            return 1;
        }
    }
    else if (c[*j] == '(')
    {
        if (c[(*j + 1)])
        {
            char *content = substr(c, (*j + 1), strlen(c)); // a revoir le strlen: fi);
            if ((type = evaluate_keyword(content)) != KW_UNKNOWN)
            {
                append(lexer, new_token_type(TOK_LPAREN));
                append(lexer, new_token_type(type));
                return 1;
            }
        }
    }
    return 0;
}

struct token *lex_assignment_word(char *c, size_t *i)
{
    if (!c || !c[*i])
        return NULL;
    if (!(c[*i] == '='))
        return NULL;
    if (*i == 0)
        return NULL;
    
    is_ass_w = true;
    size_t prev_sep = get_previous_separator_index(c, *i);
    char *var_name = substr(c, prev_sep, *i - prev_sep);
    struct token *token = new_token_type(TOK_ASS_WORD);
    token->value = var_name;
    return token;
}

int lex_separator(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    if (is_separator(c[*j]))
    {
        int type;
        char tmp[2];
        tmp[0] = c[*j];
        tmp[1] = '\0';
        int separator = evaluate_token(tmp);
        if ((type = evaluate_keyword(buffer->buf)) != KW_UNKNOWN)
        {
            if (is_word)
                append(lexer, new_token_word(buffer->buf));
            else
                append(lexer, new_token_type(type));
            append(lexer, new_token_type(separator));
            flush(buffer);
            is_word = false;
            return -1;
        }
        is_word = false;
        is_ass_w = false;
    }
    return 0;
}

int lex_parameter(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    if (c[*j] == '$' && c[*j+1] && c[*j+1] == '{')
    {
        append_word_if_needed(lexer, buffer);
        size_t close_index = c[*j+1] == '{' ? get_next_close_curl_index(c, *j) : get_next_close_parent_index(c, *j);
        char *content = substr(c, *j, close_index - *j);
        append(lexer, new_token_word(content));
        *j += strlen(content) - 1;
        flush(buffer);
        return -1;
    }
    return 0;
}

/* 
* echo "a && b"
* echo $(ls pwd  tree .)
*/
int lex_multi_token(struct lexer *lexer, struct buffer *buffer, char **splitted, int *i, size_t *j)
{
    char *c = splitted[*i];
    if (!c)
        return 1;
    if (c[*j] == '"' || c[*j] == '\'' || (c[*j] == '$' && c[*j+1] && c[*j+1] == '('))
    {
        append_word_if_needed(lexer, buffer);
        bool wait_doub_quote = c[*j] == '"';
        bool wait_simp_quote = c[*j] == '\'';
        bool wait_close_paren = c[*j] == '$' && c[*j+1] && c[*j+1] == '(';
        int stuck_left_paren_count = 0;
        int stuck_right_paren_count = 0;
         /* only for $(): add max 2 (( at the start of word */
        if (wait_close_paren)
        {
            append_buffer(buffer, '$');
            (*j)++;
        }
        while (wait_close_paren && c[*j] && c[*j] == '(')
        {
            if (stuck_left_paren_count < 2)
                append_buffer(buffer, '(');
            stuck_left_paren_count++;
            (*j)++;
        }
        /* only for " and ' => ignore theses caracters */
        if (c[*j] == '"' || c[*j] == '\'')
            (*j)++;
        /* start at ' " $ */
        while (splitted[*i])
        {
            c = splitted[*i];
            for (; *j < strlen(splitted[*i]); (*j)++)
            {
                if ((wait_doub_quote && c[*j] == '"') ||
                    (wait_simp_quote && c[*j] == '\'') ||
                    (wait_close_paren && c[*j] == ')'))
                {
                    /* only for $(): add max 2 )) at the end of word */
                    while (wait_close_paren && c[*j] && c[*j] == ')')
                    {
                        if (stuck_right_paren_count < 2)
                            append_buffer(buffer, ')');
                        stuck_right_paren_count++;
                        (*j)++;
                    }
                    /* if nb of right parenthesis is > than left ones, error */
                    if (stuck_left_paren_count < stuck_right_paren_count)
                        return 1;
                    /* TODO: when left parenthesis > right ones, should wait input */ 
                    /* need to decrement j because of while loop */
                    if (wait_close_paren) 
                        (*j)--;
                    append_buffer(buffer, '\0');
                    append(lexer, new_token_word(buffer->buf));
                    flush(buffer);
                    return -1;
                }
                append_buffer(buffer, c[*j]);
            }
            if (splitted[*i+1])
                append_buffer(buffer, ' ');
            *j = 0;
            (*i)++;
        }
    }
    return 0;
}

int lex_part(struct lexer *lexer, struct buffer *buffer, char *c, size_t *j)
{
    struct token *token = NULL;
    if ((token = lex_io_number(c, j))) { }
    else if ((token = lex_great_less_and(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        (*j)++;
    }
    else if ((token = lex_great_less(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type != TOK_LESS && token->type != TOK_GREAT) /* <, > */
        {
            if (token->type == TOK_DLESSDASH) /* <<- */
                (*j) += 2;
            else /* <<, >| ... */
                (*j)++;
        }
    }
    else if ((token = lex_comments(c, *j))) { }
    else if ((token = lex_uni_character(c, *j)))
    {
        append_word_if_needed(lexer, buffer);
        if (token->type == KW_DSEMI) /* ;; */
            (*j)++;
    }
    else if (!is_ass_w && (token = lex_assignment_word(c, j))) {
        append(lexer, token);
        flush(buffer);
    }
    append(lexer, token);
    return token ? -1 : 0;
}

bool init_lexer(struct lexer *lexer)
{
    char **splitted = split(lexer->input);
    int i = 0;
    int type;
    int kw;
    while (splitted[i])
    {
        struct buffer *buffer = new_buffer();
        char *c = splitted[i];
        is_ass_w = false;

        if ((type = evaluate_token(c)) == TOK_WORD)
        {
            for (size_t j = 0; j < strlen(c); j++)
            {
                if ((type = lex_multi_token(lexer, buffer, splitted, &i, &j)) == -1)
                {
                    c = splitted[i];
                    continue;
                }
                else if (type == 1)
                { // lex_multi_token retourne jamais 1
                    error("Lexer error: invalid parenthesis"); // mais ça s'arrête quand même avec le error non ? aah ok
                    update_last_status(2);
                    return false;
                }
                if ((type = lex_separator(lexer, buffer, c, &j)) == -1)
                    continue;
                if ((type = lex_parenthesis(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;
                
                if ((type = lex_parameter(lexer, buffer, c, &j)) == -1)
                    continue;
                if ((type = lex_part(lexer, buffer, c, &j)) == -1)
                    continue;
                else if (type == 1)
                    break;

                append_buffer(buffer, c[j]);
                if (j == strlen(c) - 1)
                {
                    if (((type = evaluate_keyword(buffer->buf)) != KW_UNKNOWN) && !is_word && !is_ass_w)
                        append(lexer, new_token_type(type));
                    else
                        append(lexer, new_token_word(buffer->buf));
                    is_word = true;
                    flush(buffer);
                }
            }
        }
        /* CASE AND IN ARE EXCEPTIONS: case a in b */
        // && kw != KW_IN && kw != KW_CASE && kw != KW_ESAC && kw != KW_FOR && kw != KW_UNTIL
        else if (((kw = evaluate_keyword(c)) != KW_UNKNOWN && is_word && (kw != KW_IN || !is_kw_in)))
            append(lexer, new_token_word(c));
        else
        {
            append(lexer, new_token_type(type));
            // if (type == TOK_NEWLINE || type == TOK_SEPAND || type == TOK_SEMI)
                is_word = false;
            if (type == KW_CASE || type == KW_FOR)
                is_kw_in = true;
            if (type == KW_IN)
                is_kw_in = false;
        }
        i++;
    }
    append(lexer, new_token_type(TOK_EOF));
    return true;
}

struct lexer *new_lexer(char *str) {
    struct lexer *lexer = xmalloc(sizeof(struct lexer));
    lexer->token_list = xmalloc(sizeof(struct token_list));
    lexer->token_list->first = NULL;
    lexer->token_list->next = NULL;
    lexer->token_list->last = NULL;
    lexer->input = str;
    bool state = init_lexer(lexer);
    return state ? lexer : NULL;
}

void free_lexer(struct lexer *lexer)
{
    if (!lexer)
        return;
    struct token *index = lexer->token_list->first;
    struct token *tmp = NULL;
    while (index)
    {
        tmp = index;
        index = index->next;
        if (tmp->type == TOK_IONUMBER || tmp->type == TOK_WORD || tmp->type == TOK_ASS_WORD)
            free(tmp->value);
        free(tmp);
    }
    if (index)
        free(index);
    free(lexer->token_list);
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
        !lexer->token_list->next)
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
        return;
    }
    lexer->token_list->last->next = token;
    lexer->token_list->last = token;
}
