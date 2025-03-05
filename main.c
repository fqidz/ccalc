#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

void parse(Token **tokensBuf, char *string, int stringSize) {
    int i = 0;
    int tokensBufIndex = 0;
    while (i < strlen(string)) {
        Token *currentToken = malloc(sizeof(Token));
        if (string[i] == 0) {
            break;
        } else if (string[i] == '+') {
            currentToken->value = "+";
            currentToken->type = PLUS;
            tokensBuf[tokensBufIndex] = currentToken;
        } else if (string[i] == '+') {
            currentToken->value = "+";
            currentToken->type = PLUS;
            tokensBuf[tokensBufIndex] = currentToken;
        }
        // bool seenDot = false;
        // char *current_token = malloc((stringSize + 1) * sizeof(char));
        // if (current_token == NULL) {
        //     // TODO: Handle error
        // }
        // int j = 0;
        // while((isdigit(string[i]) || string[i] == '.') && !seenDot) {
        //     current_token[j] = string[i];
        //     j++;
        // }
        i++;
    }
}
// 1.29+3
// check 0;
// : is digit
// check 1;
// : is dot
// check 2;
// : is digit
// check 3;
// : is digit
// check 4;
// : not digit
// save 0-3 as digit
//

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

