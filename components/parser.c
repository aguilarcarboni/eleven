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
    }
    else if ((*tokens)->type == TOKEN_SCOPY) {
        (*tokens)++;
        word = copyString(tokens, variables);
    }
    else if ((*tokens)->type == TOKEN_CONCAT) {
        (*tokens)++;
        concatStrings(tokens, variables);
    }
    else if ((*tokens)->type == TOKEN_IDENTIFIER) {
        Variable var = findVariable(tokens, variables);
        word = var.word;
        value = var.value;
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

        if ((*tokens + 1)->type == TOKEN_INDEXER && strcmp(var.type, "string") == 0) {
            int index = ((*tokens + 1)->value)[1] - '0';
            printf("%s\n", (*var.word + index) + '\0');
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
    else if ((*tokens)->type == TOKEN_PI) {
        return 3;
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