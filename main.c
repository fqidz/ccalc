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
    LEFT_PAREN,
    RIGHT_PAREN,
    INVALID,
    INVALID_NUMBER,
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
    int startNumber = 0;
    int numberLength = 0;
    bool isCurrentlyNumber = false;
    bool seenDot = false;
    bool invalidNumber = false;
    while (i < strlen(string)) {
        printf("i: %i\n", i);
        Token *currentToken = malloc(sizeof(Token));
        if (isdigit(string[i])) {
            if (isCurrentlyNumber) {
                numberLength++;
            } else {
                isCurrentlyNumber = true;
                numberLength++;
                startNumber = i;
            }

            i++;
            continue;
        }

        printf("number length: %i\n", numberLength);

        if (string[i] == '.') {
            if (seenDot) {
                numberLength++;
                invalidNumber = true;
            } else {
                seenDot = true;
                numberLength++;

                i++;
                continue;
            }
        }

        if (isCurrentlyNumber || invalidNumber) {
            // We either got the entire number or it's invalid
            currentToken->value = malloc((numberLength + 2) * sizeof(char));
            strncpy(currentToken->value, string + startNumber, numberLength);
            if (invalidNumber) {
                currentToken->type = INVALID_NUMBER;
            } else {
                currentToken->type = NUMBER;
            }

            startNumber = 0;
            numberLength = 0;
            isCurrentlyNumber = false;
            seenDot = false;
            invalidNumber = false;

            i++;
            continue;
        }

        currentToken->value = malloc(2 * sizeof(char));
        sprintf(currentToken->value, "%c", string[i]);

        switch (string[i]) {
            case '+':
                currentToken->type = PLUS;
                break;
            case '-':
                currentToken->type = MINUS;
                break;
            case '*':
            case 'x':
            case 'X':
                currentToken->type = MULTIPLY;
                break;
            case '/':
                currentToken->type = DIVIDE;
                break;
            case '(':
                currentToken->type = LEFT_PAREN;
                break;
            case ')':
                currentToken->type = RIGHT_PAREN;
                break;
            default:
                currentToken->type = INVALID;
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
            case LEFT_PAREN:
                printf("LEFT_PAREN");
                break;
            case RIGHT_PAREN:
                printf("RIGHT_PAREN");
                break;
            case INVALID:
                printf("INVALID");
                break;
            case INVALID_NUMBER:
                printf("INVALID_NUMBER");
                break;
            default:
                break;
        }
        printf("\n");
    }


    return 0;
}
