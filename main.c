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
    int currentTokenIndex = 0;
    int startNumber = 0;
    while (i < strlen(string)) {
        // printf("i: %i\n", i);
        Token *currentToken = malloc(sizeof(Token));
        switch (string[i]) {
            case '+':
                currentToken->value = "+";
                currentToken->type = PLUS;
                break;
            case '-':
                currentToken->value = "-";
                currentToken->type = MINUS;
                break;
            case '*':
            case 'x':
            case 'X':
                currentToken->value = "*";
                currentToken->type = MULTIPLY;
                break;
            case '/':
                currentToken->value = "/";
                currentToken->type = DIVIDE;
                break;
            case '(':
                currentToken->value = "(";
                currentToken->type = LEFT_PAREN;
                break;
            case ')':
                currentToken->value = ")";
                currentToken->type = RIGHT_PAREN;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                if (
                    i + 1 < strlen(string) && (
                        isdigit(string[i + 1]) ||
                        string[i + 1] == '.'
                    )
                ) {
                    startNumber = i;
                } else {

                }
                currentToken->value = malloc(2 * sizeof(char));
                sprintf(currentToken->value, "%c", string[i]);

                currentToken->type = NUMBER;
                break;
            default:
                currentToken->value = malloc(2 * sizeof(char));
                sprintf(currentToken->value, "%c", string[i]);

                currentToken->type = INVALID;
        }
        tokensBuf[currentTokenIndex] = currentToken;
        currentTokenIndex++;
        i++;
    }
}

void expandString(char *string) {
    char *buffer = realloc(string, strlen(string) * 2 + 1);
    string = buffer;
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
            case NUMBER:
                printf("NUMBER");
                break;
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
                printf("TOKEN NOT FOUND");
                break;
        }
        printf("\n");
    }

    for (int i = 0; i < sizeof(tokensBuf) / sizeof(tokensBuf[0]); i++) {
        free(tokensBuf[i]->value);
        free(tokensBuf[i]);
    }

    return 0;
}
