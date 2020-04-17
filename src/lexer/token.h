#ifndef TOKEN_H
#define TOKEN_H

#define MAX_TOKEN 256

/**
** \brief Type of a token (operators, value, ...)
*/
enum token_type
{
    TOK_ERROR,      //                                      0
    TOK_NEWLINE,	// \n                                   1
    TOK_EOF,		// EOF                                  2
    TOK_AND,		// &&                                   3
    TOK_SEPAND,		// &                                    4
    TOK_OR,		    // ||                                   5
    TOK_PIPE,		// |                                    6
    TOK_SEMI,		// ;                                    7   
    TOK_LPAREN,		// (                                    8
    TOK_RPAREN,		// )                                    9
    TOK_LCURL,		// {                                    10
    TOK_RCURL,		// }                                    11
    TOK_DLESSDASH,	// <<-                                  12
    TOK_DLESS,		// <<                                   13
    TOK_LESSGREAT,	// <>                                   14
    TOK_LESSAND,	// <&                                   15
    TOK_LESS,		// <                                    16
    TOK_DGREAT,		// >>                                   17
    TOK_GREATAND,	// >&                                   18
    TOK_CLOBBER,	// >|                                   19
    TOK_ASS_WORD,   // =                                    20
    TOK_GREAT,		// >                                    21
    TOK_IONUMBER,	// number juste before '>' or '<'       22
    TOK_NOT,        // !                                    23
    TOK_COMM,       // #                                    24
    TOK_WORD,		// all others                           25

    KW_IF,          // if                                   26
    KW_THEN,        // then                                 27
    KW_ELSE,        // else                                 28
    KW_ELIF,        // elif                                 29
    KW_FI,          // fi                                   30
    KW_DO,          // do                                   31
    KW_DONE,        // done                                 32
    KW_FOR,         // for                                  33
    KW_WHILE,       // while                                34
    KW_UNTIL,       // until                                35
    KW_CASE,        // case                                 36
    KW_ESAC,        // esac                                 37
    KW_IN,          // in                                   38
    KW_DSEMI,       // ;;                                   39
    KW_UNKNOWN      // ??                                   40
    /*...*/
};

/**
** \brief Token struct declaration.
**
** \param type the enum associated to the string.
** \param value of a token (string) if this token is a word.
** \param next pointer to the next token in the list.
*/
struct token
{
    enum token_type type;
    char *value;
    struct token *next;
};

/**
** \brief Basically a lined-list of tokens.
**
** \param first token of the list (used as start point for parsing).
** \param last token of the list.
** \param next pointer to the next token in the list.
*/
struct token_list
{
    struct token *last;
    struct token *first;
    struct token *next;
};

/**
** \brief Token allocator and initializer.
**
** \return a pointer to the allocated token.
*/
struct token *new_token(void);
struct token *new_token_type(int type);
struct token *new_token_io_number(char number);
struct token *new_token_word(char *value);
struct token *new_token_error(char *err);

int is_type(struct token *token, unsigned int type);

/**
** \brief Wrapper to release memory of a token
**
** \param token the token to free
*/
void free_token(struct token *token);

#endif /* ! TOKEN_H */
