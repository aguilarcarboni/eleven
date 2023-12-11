// Math functions

// Takes an integer and returns the truncted sin result back
// Also prints the float value
// Angle in degrees
// int sin(angle)
int esin(Token** tokens, Variable* variables) {
    double result = (double)parseExpression(tokens, variables);
    result = sin(result);
    printf("Float result: %lf\n", result);
    return result;
}

// Takes an integer and returns the truncted cos result back
// Also prints the float value
// Angle in degrees
// int cos(angle)
int ecos(Token** tokens, Variable* variables) {
    double result = (double)parseExpression(tokens, variables);
    result = cos(result);
    printf("Float result: %lf\n", result);
    return result;
}

// Takes an integer and returns the truncted tan result back
// Also prints the float value
// Angle in degrees
// int tan(angle)
int etan(Token** tokens, Variable* variables) {
    double result = (double)parseExpression(tokens, variables);
    result = cos(result);
    printf("Float result: %lf\n", result);
    return result;
}