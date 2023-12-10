// Lexer

// Function declarations
Token getNextToken(char* input, int* position);

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

        // Initialize token value (MAX LINE LENGTH)
        char value[200];

        // Copy number at start with length of number
        strncpy(value, &input[start], length);

        // add NULL char
        value[length] = '\0';

        // Check if string has .

        return (Token) { TOKEN_INT, strdup(value) };


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

        // Initialize token value (MAX LINE LENGTH)
        char value[200];

        // Copy number at start with length of number
        strncpy(value, &input[start], length);

        // add NULL char
        value[length] = '\0';

        if (strcmp(value, "if") == 0) {
            (*position)++;
            return (Token) { TOKEN_IF, "if" };
        }
        else if (strcmp(value, "else") == 0) {
            (*position)++;
            return (Token) { TOKEN_ELSE, "else" };
        }

        return (Token) { TOKEN_IDENTIFIER, strdup(value) };
    }

    // If the current character is not recognized, return an error token
    printf("Error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}