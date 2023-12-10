// Types
// Token types
typedef enum {
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_EQUALS,
    TOKEN_NL,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_SOFUNC,
    TOKEN_EOFUNC,
    TOKEN_POINT
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// Variable structure to store variable name and value
typedef struct {
    char* name;
    int value;
} Variable;