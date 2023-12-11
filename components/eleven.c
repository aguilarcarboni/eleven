#define MAX_LINE_SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "types.c"
#include "lexer.c"
#include "parser.c"
#include "math.c"
#include "string.c"
#include "sys.c"

// System function declarations
void writeToConsole(Token** tokens, Variable* variables);
Variable findVariable(Token** tokens, Variable* variables);

// Lexer function declarations
Token getNextToken(char* line, int* position, char** value);

// Parser function declarations
int parseFactor(Token** tokens, Variable* variables);
int parseTerm(Token** tokens, Variable* variables);
int parseExpression(Token** tokens, Variable* variables);
Variable parseVariableDeclaration(Token** tokens, Variable* variables);

// String methods declarations
void concatStrings(Token** tokens, Variable* variables);
char* copyString(Token** tokens, Variable* variables);

// Math library declarations
int esin(Token** tokens, Variable* variables);
int ecos(Token** tokens, Variable* variables);
int etan(Token** tokens, Variable* variables);

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
            }
            // If line is a log statement
            else if (tokens[0].type == TOKEN_LOG) {
                tokens++;
                writeToConsole(&tokens, variables);
            }
            // If line is a concat statement
            else if (tokens[0].type == TOKEN_CONCAT) {
                tokens++;
                concatStrings(&tokens, variables);
            }
            // If line is a sin statement
            else if (tokens[0].type == TOKEN_SIN) {
                tokens++;
                result = esin(&tokens, variables);
            }
            // If line is a cos statement
            else if (tokens[0].type == TOKEN_COS) {
                tokens++;
                result = ecos(&tokens, variables);
            }
            // If line is a tan statement
            else if (tokens[0].type == TOKEN_TAN) {
                tokens++;
                result = etan(&tokens, variables);
            }
            // If line is a comment
            else if (tokens[0].type == TOKEN_COMMENT) {
                tokens++;
            }
            // If line is an end of function line
            else if (tokens[0].type == TOKEN_EOFUNC) {
                tokens++;
            }
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