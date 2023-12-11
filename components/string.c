// String functions

// Concatenates two strings
// Returns a string that can be assigned to a variable
// concat(dest, src)
void concatStrings(Token** tokens, Variable* variables) {
    if ((*tokens)->type == TOKEN_IDENTIFIER && (*tokens + 1)->type == TOKEN_SEPARATOR && (*tokens + 2)->type == TOKEN_IDENTIFIER) {
        char* string1 = findVariable(tokens, variables).word;
        (*tokens) += 2;
        char* string2 = findVariable(tokens, variables).word;
        strcat(string1, string2);
        return;
    }
    else {
        printf("Error: Unexpected call. Expecting concat(dest, src). \n");
        exit(EXIT_FAILURE);
    }
}

// Copies a string into a variable
// string copy(src)
char* copyString(Token** tokens, Variable* variables) {
    if ((*tokens)->type == TOKEN_IDENTIFIER && (*tokens + 1)->type == TOKEN_RPAREN) {
        char* string = findVariable(tokens, variables).word;
        return string;
    }
    else if ((*tokens)->type == TOKEN_IDENTIFIER && (*tokens + 1)->type == TOKEN_INDEXER && (*tokens + 2)->type == TOKEN_RPAREN) {
        int index = ((*tokens + 1)->value)[1] - '0';
        char* word = findVariable(tokens, variables).word;
        if (index >= strlen(word)) {
            printf("Error: Index is out of bounds. \n");
            exit(EXIT_FAILURE);
        }
        char* value = malloc(2);
        strncpy(value, &word[index], 1);
        value[2] = '\0';
        return value;
    }
    else {
        printf("Error: Unexpected call. Expecting copy(src). \n");
        exit(EXIT_FAILURE);
    }
}