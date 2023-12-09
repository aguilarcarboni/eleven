#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <crtdbg.h>

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

int parseFactor(Token** tokens, Variable* variables);
int parseTerm(Token** tokens, Variable* variables);
int parseExpression(Token** tokens, Variable* variables);

Variable parseVariableDeclaration(Token** tokens, Variable* variables) {

    // Variable type (e.g., int)
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        printf("Error: Expected variable type\n");
        exit(EXIT_FAILURE);
    }
    char* variableType = (*tokens)->value;
    (*tokens)++;

    // Variable name
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        printf("Error: Expected variable name\n");
        exit(EXIT_FAILURE);
    }
    char* variableName = (*tokens)->value;
    (*tokens)++;

    // Equals sign
    if ((*tokens)->type != TOKEN_EQUALS) {
        printf("Error: Expected equals sign (=)\n");
        exit(EXIT_FAILURE);
    }
    (*tokens)++;

    // Value (an integer in this case) -- parseExpression()
    //int value = atoi((*tokens)->value);
    int value = parseExpression(tokens, variables);
    (*tokens)++;

    return (Variable) { variableName, value }; // possible leak
}

// Returns an integer that can be used for a further operations     (expression), x, 2
int parseFactor(Token** tokens, Variable* variables) {
    Token* currentToken = *tokens;
    if (currentToken->type == TOKEN_INT) {
        int result = atoi(currentToken->value);
        (*tokens)++;
        return result;
    }
    else if (currentToken->type == TOKEN_LPAREN) {
        (*tokens)++;
        int result = parseExpression(tokens, variables); // parse expression inside parentheses
        if ((*tokens)->type != TOKEN_RPAREN) {
            printf("Error: Missing closing parenthesis\n");
            exit(EXIT_FAILURE);
        }
        (*tokens)++; // Move this line to after checking for RPAREN
        return result;
    }
    else if (currentToken->type == TOKEN_IDENTIFIER) {
        // Variable reference, not a literal value
        // You can add your logic here to retrieve the value of the variable from memory
        // For simplicity, let's assume all variables are initialized and set to an integer value
        int result = variables[0].value;
        (*tokens)++;
        return result;
    }
    else {
        printf("Error: Unexpected token\n");
        exit(EXIT_FAILURE);
    }
}


// Returns an int that can be used to solve expression      2*2, 2*5, (5*2)/2
int parseTerm(Token** tokens, Variable* variables) {
    int result = parseFactor(tokens, variables);

    while ((*tokens)->type == TOKEN_MUL || (*tokens)->type == TOKEN_DIV) {
        Token* currentToken = *tokens;

        (*tokens)++;

        int nextFactor = parseFactor(tokens, variables);

        if (currentToken->type == TOKEN_MUL) {
            result *= nextFactor;
        }
        else if (currentToken->type == TOKEN_DIV) {
            if (nextFactor == 0) {
                printf("Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            result /= nextFactor;
        }
    }

    return result;
}

// Returns the result to the expression     (10/2) + 1, 1+1, ((3*5+1)/10)+10
int parseExpression(Token** tokens, Variable* variables) {
    int result = parseTerm(tokens, variables);

    while ((*tokens)->type == TOKEN_PLUS || (*tokens)->type == TOKEN_MINUS) {
        Token* currentToken = *tokens;

        (*tokens)++;

        int nextTerm = parseTerm(tokens, variables);

        if (currentToken->type == TOKEN_PLUS) {
            result += nextTerm;
        }
        else if (currentToken->type == TOKEN_MINUS) {
            result -= nextTerm;
        }
    }

    return result;
}

// Lexer

Token getNextToken(char* input, int* position);

// Returns the next token in the .agp file
Token getNextToken(char* input, int* position) {
    char currentChar = input[*position];

    // Skip whitespace characters
    while (isspace(currentChar)) {
        (*position)++;
        currentChar = input[*position];
    }

    if (currentChar == '\0') {
        return (Token) { TOKEN_EOF, NULL };
    }
    else if (currentChar == '\n') {
        (*position)++;
        return (Token) { TOKEN_NL, "\n" };
    }
    else if (currentChar == '+') {
        (*position)++;
        return (Token) { TOKEN_PLUS, "+" };
    }
    else if (currentChar == '-') {
        (*position)++;
        return (Token) { TOKEN_MINUS, "-" };
    }
    else if (currentChar == '*') {
        (*position)++;
        return (Token) { TOKEN_MUL, "*" };
    }
    else if (currentChar == '/') {
        (*position)++;
        return (Token) { TOKEN_DIV, "/" };
    }
    else if (currentChar == '(') {
        (*position)++;
        return (Token) { TOKEN_LPAREN, "(" };
    }
    else if (currentChar == ')') {
        (*position)++;
        return (Token) { TOKEN_RPAREN, ")" };
    }
    else if (currentChar == '=') {
        (*position)++;
        return (Token) { TOKEN_EQUALS, "=" };
    }
    else if (isdigit(currentChar)) { // if char is a number

        int start = *position;
        while (isdigit(input[*position])) { // read whole number
            (*position)++;
        }

        int length = *position - start;
        char* value = (char*)malloc(length + 1);
        if (value == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strncpy(value, &input[start], length);
        value[length] = '\0';
        return (Token) { TOKEN_INT, value };

    }
    else if (isalpha(currentChar)) { // if char is letter

        int start = *position;

        while (isalnum(input[*position])) { // read whole word
            (*position)++;
        }

        int length = *position - start; // splice string

        //malloc: *** error for object 0x7f8ebd705b20: pointer being freed was not allocated

        char value[200];

        strncpy(value, &input[start], length); // Copy string at start with length of number

        value[length] = '\0'; // add null char

        return (Token) { TOKEN_IDENTIFIER, value };
    }

    // If the current character is not recognized, return an error token
    printf("Error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}

int main() {
    char line[200];
    int variableCount = 0;
    int result = 0;

    // Initialize memory for one variable
    Variable* variables = malloc(sizeof(Variable));
    if (variables == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize memory for tokens
    Token* tokens = malloc(sizeof(Token));
    if (tokens == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read file
    FILE* file;
    file = fopen("program.agp", "r");
    if (file == NULL) {
        printf("Error: Couldn't open file\n");
        return (-1);
    }

    // Read lines
    while (fgets(line, 199, file)) {
        printf("%s", line);
        int position = 0;
        int tokenCount = 0;

        line[strlen(line)] = '\0';

        void* tokensLocation = realloc(tokens, sizeof(Token) * strlen(line));

        // Tokenize line
        Token nextToken = getNextToken(line, &position);
        while (nextToken.type != TOKEN_EOF) {
            tokens[tokenCount++] = nextToken;
            nextToken = getNextToken(line, &position);
        }

        // Parse tokens
        if (tokens[0].type == TOKEN_IDENTIFIER && tokens[1].type == TOKEN_IDENTIFIER) {
            variables[0] = parseVariableDeclaration(&tokens, variables);
        }
        else {
            result = parseExpression(&tokens, variables);
        }
        // Update tokens pointer after parsing the expression
        if (tokensLocation != NULL) {
            tokens = tokensLocation;
        }
    }
    printf("\n%d", result);

    free(tokens);
    free(variables);
    fclose(file);

    //_CrtDumpMemoryLeaks();
    return 0;
}