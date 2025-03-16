#include "logging.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parse.h"

#define STRING_SIZE 2000

int main(void)
{
    char *input_string = calloc(STRING_SIZE, sizeof(char));

    printf("Input: ");
    if (!fgets(input_string, STRING_SIZE, stdin))
        return 1;

    input_string[strcspn(input_string, "\n")] = 0;

    InputStream input_stream = { 0 };
    input_init(&input_stream, input_string);

    TokenArr tokens = { 0 };
    Error tokenize_error = input_tokenize(&tokens, &input_stream);
    if (tokenize_error.type != NO_ERROR) {
        fprintf(stderr, "%s\n", error_to_string(tokenize_error));
        return 1;
    }

    Error postfix_error = tokens_to_postfix(&tokens, input_stream.string);
    if (postfix_error.type != NO_ERROR) {
        fprintf(stderr, "%s\n", error_to_string(postfix_error));
        return 1;
    }

    double result = 0.0;
    Error evaluate_error = evaluate_postfix_tokens(&result, &tokens);
    if (evaluate_error.type != NO_ERROR) {
        fprintf(stderr, "%s\n", error_to_string(evaluate_error));
        return 1;
    }

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
