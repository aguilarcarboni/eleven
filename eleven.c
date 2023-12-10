#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.c"
#include "lexer.c"
#include "parser.c"

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
            tokens[tokenCount++] = nextToken;
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