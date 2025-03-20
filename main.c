#include "logging.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parse.h"

#define STRING_SIZE 2000

// TODO: add trigonometric functions
// TODO: add constants; eg. pi, e
// TODO: add sqrt, cube root, generic root
// TODO: multiply using brackets; eg. (2)(1+3)
// TODO: add option to show step by step
// TODO: take input from args

int main(void)
{
    char *input_string = calloc(STRING_SIZE, sizeof(char));

    printf("Input: ");
    if (!fgets(input_string, STRING_SIZE, stdin))
        return 1;
    input_string[strcspn(input_string, "\n")] = 0;

    if (strlen(input_string) == 0) {
        return 1;
    }

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
    Error evaluate_error =
            evaluate_postfix_tokens(&result, &tokens, input_stream.string);
    if (evaluate_error.type != NO_ERROR) {
        fprintf(stderr, "%s\n", error_to_string(evaluate_error));
        return 1;
    }

    printf("Result: %s\n", double_format_to_string(result));

    return 0;
}
