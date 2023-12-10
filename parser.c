// Parser

// Function declarations
int parseFactor(Token** tokens, Variable* variables);
int parseTerm(Token** tokens, Variable* variables);
int parseExpression(Token** tokens, Variable* variables);

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

    Variable result = {variableName, value};
    free(variableName);

    return result; // possible leak
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
    } else {
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