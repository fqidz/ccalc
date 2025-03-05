#include <ctype.h>
#include <stdio.h>
#include <string.h>

enum TokenType {
    DIGIT,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
};

struct Token {
    char *value;
    enum TokenType type;
};

struct Token* parse(char* string) {
    enum TokenType tokens[strlen(string)];
    int i = 0;
    while (i < strlen(string)) {
        if (isdigit(string[i])) {
            tokens[i] = DIGIT;
        } else if ('+') {
            tokens[i] = PLUS;
        } else if ('-') {
            tokens[i] = MINUS;
        }
        i++;
    }
}

void tests() {
}

int main() {
    tests();
    char input[20];

    printf("Input: ");
    // TODO: handle null
    (void) fgets(input, sizeof(input) / sizeof(input[0]), stdin);

    input[strcspn(input, "\n")] = 0;
}

