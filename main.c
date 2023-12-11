#define _CRTDBG_MAP_ALLOC
#define MAX_LINE_SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
    TOKEN_LOG,
    TOKEN_CONCAT,
    TOKEN_SCOPY,
    TOKEN_INDEXER,
    TOKEN_SIN
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

// Main function declarations
Variable findVariable(Token** tokens, Variable* variables);
Token getNextToken(char* line, int* position, char** value);
int parseFactor(Token** tokens, Variable* variables);
int parseTerm(Token** tokens, Variable* variables);
int parseExpression(Token** tokens, Variable* variables);
Variable parseVariableDeclaration(Token** tokens, Variable* variables);
// Find variable

// System function declarations
void writeToConsole(Token** tokens, Variable* variables);

// String methods declarations
void concatStrings(Token** tokens, Variable* variables);
char* copyString(Token** tokens, Variable* variables);

// float type

// Splice string
// Slice string

// while ()
// [] an int arr

// simple math
int esin(Token** tokens, Variable* variables);

/*
int ecos(Token** tokens, Variable* variables);
int etan(Token** tokens, Variable* variables);
int atan(Token** tokens, Variable* variables);
int acos(Token** tokens, Variable* variables);
int atan(Token** tokens, Variable* variables);

int floor(Token** tokens, Variable* variables);
int ceil(Token** tokens, Variable* variables);

int sqrt(Token** tokens, Variable* variables);
int log(Token** tokens, Variable* variables);
*/

int esin(Token** tokens, Variable* variables) {
    if ((*tokens)->type == TOKEN_IDENTIFIER  && (*tokens + 1)->type == TOKEN_RPAREN) {
        double result = (double) parseExpression(tokens, variables);
        result = cos(result);
        printf("Float result: %lf\n", result);
        return result;
    } else {
        printf("Error: Unexpected call. Expected sin(angle). \n");
        exit(EXIT_FAILURE);
    }
}

// Finds a variable given a position in the tokens array and the variables array
Variable findVariable(Token** tokens, Variable* variables) {
    // Declare variable index
    int variableIndex = -1;

    // Find variable in variables array
    for (int i = 0; i < 10; i++) {

        // If found
        if (strcmp(variables[i].name, (*tokens)->value) == 0) {

            // Set variable index to that variable
            variableIndex = i;
            break;
        }
    }

    // If variable index didn't change
    if (variableIndex == -1) {
        printf("Error: Variable not found\n");
        exit(EXIT_FAILURE);
    }
    return (variables[variableIndex]);
}

// Parser

// Parses a variables declaration
// Returns a variable that can be stored in variable array
// Examples: int x = 2, int y = x
Variable parseVariableDeclaration(Token** tokens, Variable* variables) {

    // Variable type
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        printf("Error: Expected variable type\n");
        exit(EXIT_FAILURE);
    }

    // Save variable type
    char* variableType = (*tokens)->value;

    // Move forward on tokens array
    (*tokens)++;

    // Variable name
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        printf("Error: Expected variable name\n");
        exit(EXIT_FAILURE);
    }

    // Save variable name
    char* variableName = (*tokens)->value;

    // Move forward on tokens array
    (*tokens)++;

    // Equals sign
    if ((*tokens)->type != TOKEN_EQUALS) {
        printf("Error: Expected equals sign (=)\n");
        exit(EXIT_FAILURE);
    }

    // Move forward on tokens array
    (*tokens)++;

    // Calculate length of token string
    int length = strlen((*tokens)->value);

    // Declare value for output
    int value = 0;

    // Allocate memory for one word of length of token
    char* word = malloc(length + 1);

    if ((*tokens)->type == TOKEN_INT || (*tokens)->type == TOKEN_STR) {
        // Check for the variable type
        // If is int
        if (strcmp(variableType, "int") == 0) {
            value = parseExpression(tokens, variables);
        }
        // If is string
        else if (strcmp(variableType, "string") == 0) {
            strncpy(word, (*tokens)->value, length);
            word[length] = '\0';
        }
        else {
            printf("Error: Unidentified variable type\n");
            exit(EXIT_FAILURE);
        }
    } else if ((*tokens)->type == TOKEN_SCOPY) {
        (*tokens)++;
        word = copyString(tokens, variables);
    }
    else if ((*tokens)->type == TOKEN_CONCAT) {
        (*tokens)++;
        concatStrings(tokens, variables);
    }
    // Move forward on tokens array
    (*tokens)++;

    return (Variable) { variableType, variableName, value, word };
}

// Parses a factor
// Returns an integer that can be used to solve a term     
// Examples: x, 2, ()
int parseFactor(Token** tokens, Variable* variables) {
    // If token is an integer
    if ((*tokens)->type == TOKEN_INT) {
        int result = atoi((*tokens)->value);

        // Move forward in tokens array
        (*tokens)++;
        return result;
    }
    // If token is a (
    else if ((*tokens)->type == TOKEN_LPAREN) {
        // Move forward in tokens array
        (*tokens)++;

        // Parse expression inside parenthesis
        int result = parseExpression(tokens, variables);

        // Check for lack of )
        if ((*tokens)->type != TOKEN_RPAREN) {
            printf("Error: Missing closing parenthesis\n");
            exit(EXIT_FAILURE);
        }

        // Move forward in tokens array
        (*tokens)++;
        return result;
    }
    else if ((*tokens)->type == TOKEN_IDENTIFIER) {
        Variable var = findVariable(tokens, variables);

        if ((*tokens+1)->type == TOKEN_INDEXER && strcmp(var.type,"string") == 0) {
            int index = ((*tokens + 1)->value)[1] - '0';
            printf("%c\n", *var.word + index);
        }

        // Assign result to that variable's value
        int result = var.value;
        if (var.word != NULL && result == 0) {
            printf("%s\n", var.word);
        }
        // Move forward in tokens array
        (*tokens)++;
        return result;
    }
    else {
        printf("Error: Unexpected token\n");
        exit(EXIT_FAILURE);
    }
}

// Parses a term
// Returns an int that can be used to solve expression      
// Examples: (2*2), ((2*3)/2)
int parseTerm(Token** tokens, Variable* variables) {
    // Parse first factor in term
    int result = parseFactor(tokens, variables);

    // Loop so you can do multiple computations
    while ((*tokens)->type == TOKEN_MUL || (*tokens)->type == TOKEN_DIV) {
        Token* currentToken = *tokens;

        // Move forward in tokens array
        (*tokens)++;

        // Parse next factor in term
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

// Parses an expression
// Returns the result to the expression     
// Examples: (10/2) + 1, 1+1, ((3*5+1)/10)+10, 1 == 2
int parseExpression(Token** tokens, Variable* variables) {
    // Parse first term in expression
    int result = parseTerm(tokens, variables);

    // Check if expression is an equality
    if ((*tokens)->type == TOKEN_EQUALS && ((*tokens) + 1)->type == TOKEN_EQUALS) {
        // Move forward two spaces in tokens array (two equals)
        (*tokens) += 2;

        // Parse next term in expression
        int nextTerm = parseTerm(tokens, variables);

        // Return a boolean equality
        return (result == nextTerm);
    }
    else {
        // Loop so you can do multiple computations
        while ((*tokens)->type == TOKEN_PLUS || (*tokens)->type == TOKEN_MINUS) {
            Token* currentToken = *tokens;

            // Move forward in tokens array
            (*tokens)++;

            // Parse next term in expression
            int nextTerm = parseTerm(tokens, variables);

            // Compute result depending on the operation
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

// Writes a single line to the "console"
void writeToConsole(Token** tokens, Variable* variables) {
    // Parse expression inside arguments
    int result = parseExpression(tokens, variables);

    // Print if expression is a string
    if (result != 0) {
        printf("%d\n", result);
    } else {
        return;
    }
}

// Concatenates two strings
// Returns a string that can be assigned to a variable
// concat(dest, src)
void concatStrings(Token** tokens, Variable* variables) {
    if ((*tokens)->type == TOKEN_IDENTIFIER  && (*tokens + 1)->type == TOKEN_SEPARATOR && (*tokens + 2)->type == TOKEN_IDENTIFIER) {
        char* string1 = findVariable(tokens, variables).word;
        (*tokens)+=2;
        char* string2 = findVariable(tokens, variables).word;
        strcat(string1, string2);
        return;
    } else {
        printf("Error: Unexpected call. Expecting concat(dest, src). \n");
        exit(EXIT_FAILURE);
    }
}

// Copies a string into a variable
// copy(src)
char* copyString(Token** tokens, Variable* variables) {
    if ((*tokens)->type == TOKEN_IDENTIFIER  && (*tokens + 1)->type == TOKEN_RPAREN) {
        char* string = findVariable(tokens, variables).word;
        return string;
    } else if ((*tokens)->type == TOKEN_IDENTIFIER  && (*tokens + 1)->type == TOKEN_INDEXER && (*tokens + 2)->type == TOKEN_RPAREN) {
        int index = ((*tokens + 1)->value)[1] - '0';
        char* word = findVariable(tokens, variables).word;
        if (index >= strlen(word)) {
            printf("Error: Index is out of bounds. \n");
            exit(EXIT_FAILURE);
        }
        char *value = malloc(2);
        strncpy(value, &word[index], 1);
        value[2] = '\0';
        return value;
    } else {
        printf("Error: Unexpected call. Expecting copy(src). \n");
        exit(EXIT_FAILURE);
    }
}

// Lexer

// Returns the next token in the .agp file
Token getNextToken(char* line, int* position, char** value) {
    // Set current char
    char currentChar = line[*position];

    // Skip whitespace and tab characters
    while (isspace(currentChar) || currentChar == '\t') {
        (*position)++;
        currentChar = line[*position];
    }

    // Tokenize specific characters
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
    else if (currentChar == ',') {
        (*position)++;
        return (Token) { TOKEN_SEPARATOR, "," };
    }
    else if (currentChar == '.') {
        (*position)++;
        return (Token) { TOKEN_PERIOD, "." };
    }
    else if (currentChar == '"') { // if char is a number
        // Index start of token
        (*position)++;
        int start = *position;

        // Read whole number
        while (line[*position] != '"' && line[*position] != '\0') {
            (*position)++;
        }
        if (line[*position] == '\0') {
            printf("Error: Unterminated string literal\n");
            exit(EXIT_FAILURE);
        }

        // Calculate length of token
        int length = *position - start;


        (*position)++;

        // Reallocate data for values depending on length of token
        char* valueLocation = realloc(*value, length + 1);

        // Update value pointer location for freeing
        if (valueLocation != NULL) {
            *value = valueLocation;
        }

        // Copy number at start with length of number
        strncpy(*value, &line[start], length);

        // add NULL char
        (*value)[length] = '\0';

        return (Token) { TOKEN_STR, strdup(*value) };

    }
    else if (currentChar == '[') { // if char is a number
        // Index start of token
        int start = *position;

        (*position)++;

        // Read whole number
        while (isdigit(line[*position])) {
            (*position)++;
        }
        if (line[*position] != ']') {
            printf("Error: Unterminated array\n");
            exit(EXIT_FAILURE);
        }

        (*position)++;

        // Calculate length of token
        int length = *position - start;

        // Reallocate data for values depending on length of token
        char* valueLocation = realloc(*value, length + 1);

        // Update value pointer location for freeing
        if (valueLocation != NULL) {
            *value = valueLocation;
        }

        // Copy number at start with length of number
        strncpy(*value, &line[start], length);

        // add NULL char
        (*value)[length] = '\0';

        return (Token) { TOKEN_INDEXER, strdup(*value) };

    }
    else if (isdigit(currentChar)) { // if char is a number

        // Index start of token
        int start = *position;

        // Read whole number
        while (isdigit(line[*position])) {
            (*position)++;
        }

        // Calculate length of token
        int length = *position - start;

        // Reallocate data for values depending on length of token
        char* valueLocation = realloc(*value, length + 1);

        // Update value pointer location for freeing
        if (valueLocation != NULL) {
            *value = valueLocation;
        }

        // Copy number at start with length of number
        strncpy(*value, &line[start], length);

        // add NULL char
        (*value)[length] = '\0';

        return (Token) { TOKEN_INT, strdup(*value) };

    }
    else if (isalpha(currentChar)) { // if char is letter

        // Index start of token
        int start = *position;

        // Read whole word
        while (isalnum(line[*position])) {
            (*position)++;
        }

        // Calculate length of token
        int length = *position - start;

        // Reallocate data for values depending on length of token
        char* valueLocation = realloc(*value, length + 1);

        // Update value pointer location for freeing
        if (valueLocation != NULL) {
            *value = valueLocation;
        }

        // Copy number at start with length of number
        strncpy(*value, &line[start], length);

        // add NULL char
        (*value)[length] = '\0';

        // Tokenize if-else tokens
        if (strcmp(*value, "if") == 0) {
            (*position)++;
            return (Token) { TOKEN_IF, "if" };
        }
        else if (strcmp(*value, "else") == 0) {
            (*position)++;
            return (Token) { TOKEN_ELSE, "else" };
        }        
        else if (strcmp(*value, "log") == 0) {
            (*position)++;
            return (Token) { TOKEN_LOG, "log" };
        }        
        else if (strcmp(*value, "concat") == 0) {
            (*position)++;
            return (Token) { TOKEN_CONCAT, "concat" };
        }
        else if (strcmp(*value, "copy") == 0) {
            (*position)++;
            return (Token) { TOKEN_SCOPY, "copy" };
        }
        else if (strcmp(*value, "esin") == 0) {
            (*position)++;
            return (Token) { TOKEN_SIN, "esin" };
        }

        return (Token) { TOKEN_IDENTIFIER, strdup(*value) };
    }

    // If the current character is not recognized, return an error token
    printf("Tokenizing error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}

int main(void) {
    // Declare variables
    char line[MAX_LINE_SIZE];
    int variableCount = 0;
    int result = 0;

    int condition = 1;

    // Condition copy is used to check previous condition for if-else statements
    int conditionCopy = condition;

    // Initialize memory for 20 variables
    Variable* variables = malloc(sizeof(Variable) * 20);
    if (variables == NULL) {
        printf("System error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read file
    FILE* file;
    file = fopen("program.el", "r");
    if (file == NULL) {
        printf("System error: Couldn't open file\n");
        exit(EXIT_FAILURE);
    }

    // Read lines
    while (fgets(line, 199, file)) {

        // Declare variables
        int position = 0;
        int tokenCount = 0;

        // Print out current line
        printf("%s", line);

        // Allocate memory for tokens
        Token* tokens = malloc(sizeof(Token) * strlen(line));
        if (tokens == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        // Save tokens location for freeing
        void* tokensLocation = tokens;

        // Tokenize line
        // Create flag for stopping tokenizing
        int flag = 1;

        // Allocate memory for values of tokens
        char* value = malloc(sizeof(char));

        // Declare next token
        Token nextToken;

        // Start tokenizing one line
        while (flag) {
            nextToken = getNextToken(line, &position, &value);
            tokens[tokenCount] = nextToken;
            tokenCount++;

            // If token is EOF or \n
            if (nextToken.type == TOKEN_EOF) {
                flag = 0;
                break;
            }
        }

        // If you are in loop mode
        // Check if condition is true now
        // Rerun line

        // Parse tokens
        // Check if you can read line
        if (condition) {

            // If line is a variable declaration
            if (tokens[0].type == TOKEN_IDENTIFIER && tokens[1].type == TOKEN_IDENTIFIER) {
                variables[variableCount] = parseVariableDeclaration(&tokens, variables);
                variableCount++;
            }

            // If line is an if statement
            else if (tokens[0].type == TOKEN_IF) {
                tokens++;
                condition = parseExpression(&tokens, variables);
            }

            // If line is an else or else if statement
            else if (tokens[0].type == TOKEN_ELSE) {
                tokens++;
                if ((*tokens).type == TOKEN_IF) {
                    tokens++;
                    condition = parseExpression(&tokens, variables);
                }
                else {
                    condition = !conditionCopy;
                }
            } else if (tokens[0].type == TOKEN_LOG) {
                tokens++;
                writeToConsole(&tokens, variables);
            }
            else if (tokens[0].type == TOKEN_CONCAT) {
                tokens++;
                concatStrings(&tokens, variables);
            }
            else if (tokens[0].type == TOKEN_SIN) {
                tokens++;
                result = esin(&tokens, variables);
            }
            // If line is an end of function line
            else if (tokens[0].type == TOKEN_EOFUNC) {
                tokens++;
            }
            // Loop logic
            // Check expression and determine loop
            // Save lines needed for loop

            // If line is an expression
            else {
                result = parseExpression(&tokens, variables);
            }
        }

        // Reset condition after finishing a function
        if (tokens[0].type == TOKEN_EOFUNC) {
            conditionCopy = condition;
            condition = 1;
        }

        // Update tokens pointer for freeing
        if (tokensLocation != NULL) {
            tokens = tokensLocation;
        }

        // Free memory allocations
        free(value);
        free(tokens);
    }

    // Display result
    printf("Result: %d\n", result);

    // Free more memory and close file
    free(variables);
    fclose(file);
    return 0;
}