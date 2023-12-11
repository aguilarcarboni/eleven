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
    else if (currentChar == '$') { // if char is a number
        // Index start of token
        int start = *position;

        (*position)++;

        // Read whole number
        while (line[*position] == '$') {
            (*position)++;
        }

        // Calculate length of token
        int length = *position - start;

        if (length != 3) {
            printf("Error: Unexpected token: $. Comment with $$$. \n");
            exit(EXIT_FAILURE);
        }

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

        return (Token) { TOKEN_COMMENT, strdup(*value) };

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
        else if (strcmp(*value, "ecos") == 0) {
            (*position)++;
            return (Token) { TOKEN_COS, "etan" };
        }
        else if (strcmp(*value, "etan") == 0) {
            (*position)++;
            return (Token) { TOKEN_TAN, "etan" };
        }
        else if (strcmp(*value, "PI") == 0) {
            (*position)++;
            return (Token) { TOKEN_PI, "PI" };
        }
        return (Token) { TOKEN_IDENTIFIER, strdup(*value) };
    }

    // If the current character is not recognized, return an error token
    printf("Tokenizing error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}