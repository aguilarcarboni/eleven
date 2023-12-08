#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper files
#include "types.c"
#include "lexer.c"
#include "parser.c"

int main() {
    char line[200];
    int variableCount = 0;
    int result;

    Token* tokens = (Token*)malloc(sizeof(Token) * strlen(line)); // initialize dynamic tokens array depending on line size
    if (tokens == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    Variable* variables = (Variable*)malloc(sizeof(Variable)); // initialize size for one variable
    if (variables == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read file
    FILE *file;
    file = fopen("program.agp", "r");
	if (file == NULL) {
		printf("Error: Couldn't open file\n");
		return (-1); 
	}

    // Read lines
    while(fgets(line, 200, file)) {
        printf("%s", line);
        int position = 0; // Pointer ??? 
        int tokenCount = 0;

        line[strlen(line)] = '\0';
        // Tokenize line
        Token nextToken = getNextToken(line, &position);
        while (nextToken.type != TOKEN_EOF) {
            tokens[tokenCount++] = nextToken;
            nextToken = getNextToken(line, &position);
        }

        // Parse tokens
        if (tokens[0].type == TOKEN_IDENTIFIER && tokens[1].type == TOKEN_IDENTIFIER) {
            variables[variableCount++] = parseVariableDeclaration(&tokens, variables);
        } else {
            result = parseExpression(&tokens, variables);
            printf("Result: %d\n", result);
        }
    }
    
    if (tokens != NULL) {
        free(tokens);
    }

    if (variables != NULL) {
        free(variables);
    }

    fclose(file);
    return 0;
}

// Declare and then change value of variable
// 
// val op (val)