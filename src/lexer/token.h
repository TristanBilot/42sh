#ifndef TOKEN_H
#define TOKEN_H

#define MAX_TOKEN 256

/**
** \brief Type of a token (operators, value, ...)
*/
enum token_type
{
    TOK_NONE,
    TOK_NEWLINE,	// \n
    TOK_EOF,		// EOF
    TOK_AND,		// &&
    TOK_SEPAND,		// &
    TOK_OR,		    // ||
    TOK_PIPE,		// |
    TOK_DSEMI,		// ;;
    TOK_SEMI,		// ;
    TOK_LPAREN,		// (
    TOK_RPAREN,		// )
    TOK_DLESSDASH,	// <<-
    TOK_DLESS,		// <<
    TOK_LESSGREAT,	// <>
    TOK_LESSAND,	// <&
    TOK_LESS,		// <
    TOK_DGREAT,		// >>
    TOK_GREATAND,	// >&
    TOK_CLOBBER,	// >|
    TOK_GREAT,		// >
    TOK_IONUMBER,	// number juste before '>' or '<'
    TOK_WORD		// all others
};

enum word_type
{
    WORD_IF,
    WORD_THEN,
    WORD_ELSE,
    WORD_ELIF,
    WORD_FI,
    WORD_DO,
    WORD_DONE,
    WORD_FOR
    /*...*/
};

/**
** \brief Token struct declaration
**
** \param type the enum associated to the string.
*/
struct token
{
    enum token_type type;
    struct token *next;
};

struct token_list
{
    struct token *last;
    struct token *first;
    struct token *next;
};

/**
** \brief Token allocator and initializer
**
** \return a pointer to the allocated token
*/
struct token *new_token(void);
struct token *new_token_type(int type);

/**
** \brief Wrapper to release memory of a token
**
** \param token the token to free
*/
void free_token(struct token *token);

#endif /* ! TOKEN_H */
