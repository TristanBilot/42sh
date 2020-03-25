#ifndef TOKEN_H
#define TOKEN_H

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
    TOK_SEP,		// ;
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
} e_tokenid;

/**
** \brief Token struct declaration
*/
struct token
{
    /* type of the token */
    enum token_type type;
    /* hold the number (only used when type == TOKEN_NUMBER) */
    int value;
};

/**
** \brief String representation of a token (used for debugging and errors)
**
** \return a string describing the given token
** \param token to stringify
*/
const char *token_str(struct token *token);

/**
** \brief Token allocator and initialiser
**
** \return a pointer to the allocated token
*/
struct token *token_alloc(void);

/**
** \brief Wrapper to release memory of a token
**
** \param token the token to free
*/
void token_free(struct token *token);

#endif /* ! TOKEN_H */
