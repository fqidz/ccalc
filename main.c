#include "logging.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parse.h"

#define STRING_SIZE 256

int main(void)
{
    char *input_string = calloc(STRING_SIZE, sizeof(char));
    TokenArr tokens = { 0 };

    printf("Input: ");
    if (!fgets(input_string, STRING_SIZE, stdin))
        return 1;

    input_string[strcspn(input_string, "\n")] = 0;

    InputStream input_stream = { 0 };
    input_init(&input_stream, input_string);

    Error tokenize_error = input_tokenize(&tokens, &input_stream);
    switch (tokenize_error.type) {
    case INVALID_CHAR:
        fprintf(stderr, "%s\n", error_to_string(tokenize_error));
        return 1;
    case NO_ERROR:
        break;
    default:
        UNREACHABLE;
    }

    tokens_to_postfix(&tokens);
    // printf("%s\n", tokenarr_to_string(&tokens));
    double result = evaluate_postfix_tokens(&tokens);

    double integral;
    double fractional = fabs(modf(result, &integral));
    if (fractional < 0.000001) {
        double result_int = round(result);
        printf("Result: %i\n", (int)result_int);
    } else {
        printf("Result: %f\n", result);
    }

    return 0;
}
