#include "logging.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parse.h"

#define STRING_SIZE 2000

// TODO: deal with negative cos, sin, e, phi, etc.
// TODO: give good error for division by zero
// TODO: (-1)^(1/2) should return NAN
// TODO: add trigonometric functions
// TODO: add constants; eg. pi, e
// TODO: add sqrt, cube root, generic root
// TODO: multiply using brackets; eg. (2)(1+3)
// TODO: add option to show step by step
// TODO: take input from args

int main(void)
{
    char *input_string = calloc(STRING_SIZE, sizeof(char));

    InputStream input_stream = { 0 };
    TokenArr tokens;
    double result = 0.0;
    char *result_formatted = NULL;

    tokenarr_init(&tokens, 2);
    printf("<ccalc>\n");
    while (1) {
        printf(">> ");
        if (!fgets(input_string, STRING_SIZE, stdin))
            return 1;
        input_string[strcspn(input_string, "\n")] = 0;

        for (int i = 0; input_string[i]; i++) {
            input_string[i] = (char)tolower(input_string[i]);
        }
        if (strcmp(input_string, "exit") == 0) {
            break;
        }

        if (strlen(input_string) == 0) {
            continue;
        }

        input_init(&input_stream, input_string);

        Error tokenize_error = input_tokenize(&tokens, &input_stream);
        if (tokenize_error.type != NO_ERROR) {
            fprintf(stderr, "%s\n", error_to_string(tokenize_error));
            continue;
        }

        Error postfix_error = tokens_to_postfix(&tokens, input_stream.string);
        if (postfix_error.type != NO_ERROR) {
            fprintf(stderr, "%s\n", error_to_string(postfix_error));
            continue;
        }

        Error evaluate_error =
                evaluate_postfix_tokens(&result, &tokens, input_stream.string);
        if (evaluate_error.type != NO_ERROR) {
            fprintf(stderr, "%s\n", error_to_string(evaluate_error));
            continue;
        }

        result_formatted = double_format_to_string(result);
        printf(" %s\n", result_formatted);

        free(result_formatted);
        tokenarr_free(&tokens);
        input_free(&input_stream);
    }

    tokenarr_free(&tokens);
    free(input_string);
    return 0;
}
