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
    ILLEGAL,
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

void parse(Token **tokensBuf, char *string, int stringSize) {
    if (stringSize == 0) {
        return;
    }

    int i = 0;
    while (i < strlen(string)) {
        Token *currentToken = malloc(sizeof(Token));
        char *currentValue = malloc(2 * sizeof(char));
        sprintf(currentValue, "%c", string[i]);

        switch (string[i]) {
            case '+':
                currentToken->value = currentValue;
                currentToken->type = PLUS;
                break;
            case '-':
                currentToken->value = currentValue;
                currentToken->type = MINUS;
                break;
            case '*':
            case 'x':
            case 'X':
                currentToken->value = currentValue;
                currentToken->type = MULTIPLY;
                break;
            case '/':
                currentToken->value = currentValue;
                currentToken->type = DIVIDE;
                break;
            default:
                currentToken->value = currentValue;
                currentToken->type = ILLEGAL;
        }
        tokensBuf[i] = currentToken;
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
    printf("Running Tests\n");
}

int main() {
    tests();
    char input[20];

    printf("Input: ");
    // TODO: handle null
    (void) fgets(input, sizeof(input) / sizeof(input[0]), stdin);
    input[strcspn(input, "\n")] = 0;

    Token *tokensBuf[strlen(input)];
    parse(tokensBuf, input, strlen(input));

    for (int i = 0; i < sizeof(tokensBuf) / sizeof(tokensBuf[0]); i++) {
        printf("%s ", tokensBuf[i]->value);
        switch (tokensBuf[i]->type) {
            case PLUS:
                printf("PLUS");
                break;
            case MINUS:
                printf("MINUS");
                break;
            case MULTIPLY:
                printf("MULTIPLY");
                break;
            case DIVIDE:
                printf("DIVIDE");
                break;
            case ILLEGAL:
                printf("ILLEGAL");
                break;
            default:
                break;
        }
        printf("\n");
    }


    return 0;
}

