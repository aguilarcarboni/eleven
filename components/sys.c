
// System functions

// Writes a single line to the "console"
void writeToConsole(Token** tokens, Variable* variables) {
    // Parse expression inside arguments
    int result = parseExpression(tokens, variables);

    // Print if expression is a string
    if (result != 0) {
        printf("%d\n", result);
    }
    else {
        return;
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