// Types

// Token types
typedef enum {
    TOKEN_INT,
    TOKEN_STR,
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
    TOKEN_SEPARATOR,
    TOKEN_PERIOD,
    TOKEN_COMMENT,
    TOKEN_LOG,
    TOKEN_CONCAT,
    TOKEN_SCOPY,
    TOKEN_INDEXER,
    TOKEN_SIN,
    TOKEN_COS,
    TOKEN_TAN,
    TOKEN_PI
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// Variable structure to store variable type, name an int and a string
// Multi type structure allows for ease of development as all variables are in one single struct array
typedef struct {
    char* type;
    char* name;
    int value;
    char* word;
} Variable;