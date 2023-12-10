#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

Token getNextToken(char* input, int* position);
int parseFactor(Token** tokens, Variable* variables);
int parseTerm(Token** tokens, Variable* variables);
int parseExpression(Token** tokens, Variable* variables);
Variable parseVariableDeclaration(Token** tokens, Variable* variables);

// Parser
//
Variable parseVariableDeclaration(Token** tokens, Variable* variables) {

    // Variable type
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
    if ((*tokens)->type == TOKEN_INT) {
        if (((*tokens) + 1)->type == TOKEN_POINT) {
            // get next next token (decimal)
            // generate string for float
            // result = atof(string)
        }
        int result = atoi((*tokens)->value);
        (*tokens)++;
        return result;
    }
    else if ((*tokens)->type == TOKEN_LPAREN) {
        (*tokens)++;
        int result = parseExpression(tokens, variables); // parse expression inside parentheses
        if ((*tokens)->type != TOKEN_RPAREN) {
            printf("Error: Missing closing parenthesis\n");
            exit(EXIT_FAILURE);
        }
        (*tokens)++; // Move this line to after checking for RPAREN
        return result;
    }
    else if ((*tokens)->type == TOKEN_IDENTIFIER) {
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

// Returns the result to the expression     (10/2) + 1, 1+1, ((3*5+1)/10)+10, 1 == 2
int parseExpression(Token** tokens, Variable* variables) {

    int result = parseTerm(tokens, variables);

    if ((*tokens)->type == TOKEN_EQUALS && ((*tokens) + 1)->type == TOKEN_EQUALS) {
        (*tokens) += 2;
        int nextTerm = parseTerm(tokens, variables);
        return (result == nextTerm);
    }
    else {
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
}

// Lexer
// Returns the next token in the .agp file
Token getNextToken(char* input, int* position) {
    char currentChar = input[*position];

    // Skip whitespace and tab characters
    while (isspace(currentChar) || currentChar == '\t') {
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
    else if (currentChar == ':') {
        (*position)++;
        return (Token) { TOKEN_SOFUNC, ":" };
    }
    else if (currentChar == '_') {
        (*position)++;
        return (Token) { TOKEN_EOFUNC, "_" };
    }
    else if (currentChar == '.') {
        (*position)++;
        return (Token) { TOKEN_POINT, "." };
    }
    else if (isdigit(currentChar)) { // if char is a number

        // Index start of token
        int start = *position;

        // Read whole number
        while (isdigit(input[*position])) {
            (*position)++;
        }

        // Calculate length of token
        int length = *position - start;

        // Create a static value
        char value[200];

        // Copy number at start with length of number
        strncpy(value, &input[start], length);

        // add NULL char
        value[length] = '\0';

        return (Token) { TOKEN_INT, _strdup(value) };


    }
    else if (isalpha(currentChar)) { // if char is letter

        // Index start of token
        int start = *position;

        // Read whole word
        while (isalnum(input[*position])) {
            (*position)++;
        }

        // Calculate length of token
        int length = *position - start;

        // Allocate memory for value
        char* value = malloc(length + 1);

        // Create a static copy of value
        char valueCopy[200];

        // Copy number at start with length of number
        strncpy(value, &input[start], length);

        // Add null char
        value[length] = '\0';

        // Copy value into static copy and free
        strcpy(valueCopy, value);
        free(value);

        if (strcmp(valueCopy, "if") == 0) {
            (*position)++;
            return (Token) { TOKEN_IF, "if" };
        }
        else if (strcmp(valueCopy, "else") == 0) {
            (*position)++;
            return (Token) { TOKEN_ELSE, "else" };
        }
        else {
            return (Token) { TOKEN_IDENTIFIER, valueCopy };
        }
    }

    // If the current character is not recognized, return an error token
    printf("Error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}

int main(void) {
    char line[200];
    int variableCount = 0;
    int result = 0;
    int condition = 1;
    int conditionCopy = condition;

    // Initialize memory for one variable
    Variable* variables = malloc(sizeof(Variable) * 20);
    if (variables == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for tokens
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
        exit(EXIT_FAILURE);
    }

    // Read lines
    while (fgets(line, 199, file)) {
        int position = 0;
        int tokenCount = 0;
        printf("%s", line);

        line[strlen(line)] = '\0';

        // Reallocate data for tokens
        void* tokensLocation = realloc(tokens, sizeof(Token) * strlen(line));

        // Tokenize line
        Token nextToken = getNextToken(line, &position);
        while (nextToken.type != TOKEN_EOF) {
            tokens[tokenCount] = nextToken;
            tokenCount++;
            nextToken = getNextToken(line, &position);
        }

        // Check if you can read line
        if (condition) {
            // Parse tokens
            if (tokens[0].type == TOKEN_IDENTIFIER && tokens[1].type == TOKEN_IDENTIFIER) {
                variables[0] = parseVariableDeclaration(&tokens, variables);
            }
            else if (tokens[0].type == TOKEN_IF) {
                tokens++;
                condition = parseExpression(&tokens, variables);
            }
            else if (tokens[0].type == TOKEN_ELSE) {
                tokens++;
                condition = !conditionCopy;
            }
            else if (tokens[0].type == TOKEN_EOFUNC) {
                //
            }
            else {
                result = parseExpression(&tokens, variables);
            }
        }

        if (tokens[0].type == TOKEN_EOFUNC) {
            conditionCopy = condition;
            condition = 1;
        }

        // Update tokens pointer after parsing the expression
        if (tokensLocation != NULL) {
            tokens = tokensLocation;
        }
    }
    printf("\n%d\n", result);
    free(tokens);
    free(variables);
    fclose(file);

    exit(EXIT_SUCCESS);
    return 0;
}