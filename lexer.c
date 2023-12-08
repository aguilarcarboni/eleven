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
        return (Token){TOKEN_EOF, NULL};
    }
    else if (currentChar == '\n') {
        (*position)++;
        return (Token){TOKEN_NL, "\n"};
    } else if (currentChar == '+') {
        (*position)++;
        return (Token){TOKEN_PLUS, "+"};
    } else if (currentChar == '-') {
        (*position)++;
        return (Token){TOKEN_MINUS, "-"};
    } else if (currentChar == '*') {
        (*position)++;
        return (Token){TOKEN_MUL, "*"};
    } else if (currentChar == '/') {
        (*position)++;
        return (Token){TOKEN_DIV, "/"};
    } else if (currentChar == '(') {
        (*position)++;
        return (Token){TOKEN_LPAREN, "("};
    } else if (currentChar == ')') {
        (*position)++;
        return (Token){TOKEN_RPAREN, ")"};
    } else if (currentChar == '=') {
        (*position)++;
        return (Token){TOKEN_EQUALS, "="};
    } else if (isdigit(currentChar)) { // if char is a number

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
        return (Token){TOKEN_INT, value};

    } else if (isalpha(currentChar)) { // if char is letter
        
        int start = *position;

        while (isalnum(input[*position])) { // read whole word
            (*position)++;
        }

        int length = *position - start; // splice string

        char* value = (char*)malloc(length + 1); // allocate space for one value
        if (value == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        strncpy(value, &input[start], length); // Copy string at start with length of number

        value[length] = '\0'; // add null char

        return (Token){TOKEN_IDENTIFIER, value};
    }

    // If the current character is not recognized, return an error token
    printf("Error: Unrecognized character '%c'\n", currentChar);
    exit(EXIT_FAILURE);
}