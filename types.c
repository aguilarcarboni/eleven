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
    TOKEN_NL
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