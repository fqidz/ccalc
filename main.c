#include "logging.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parse.h"

#define STRING_SIZE 2000

int double_get_decimal_length(double input, int max_len)
{
    double _integrand;
    double fractional = fabs(modf(input, &_integrand));
    size_t needed = (size_t)snprintf(NULL, 0, "%.*f", max_len, fractional) + 1;
    size_t first_non_zero_index = 0;
    char *buffer = malloc(needed);
    sprintf(buffer, "%.*f", max_len, fractional);
    // start after decimal
    for (size_t i = 2; i < strlen(buffer); i++) {
        if (buffer[i] != '0') {
            first_non_zero_index = i;
            break;
        }
    }

    // start from the end
    for (size_t i = strlen(buffer) - 1; i > first_non_zero_index; i--) {
        if (buffer[i] != '0') {
            return (int)i - 1;
        }
    }

    LOG_ASSERT(first_non_zero_index > 0);
    return (int)first_non_zero_index - 1;
}

double double_get_scientific_notation(double input, int *exponent)
{
    *exponent = (int)(floor(log10(fabs(input))));
    // *exponent = tmp_exponent > 0 ? tmp_exponent :
    //                                (int)(floor(log10(fabs(input))) - 1);
    return input / pow(10, *exponent);
}

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
    Error evaluate_error = evaluate_postfix_tokens(&result, &tokens);
    if (evaluate_error.type != NO_ERROR) {
        fprintf(stderr, "%s\n", error_to_string(evaluate_error));
        return 1;
    }

    int decimals_len = 0;
    int exponent;
    double mantissa = double_get_scientific_notation(result, &exponent);
    if (exponent >= 12) {
        decimals_len = double_get_decimal_length(mantissa, 7);
        printf("Result: %.*fe+%d\n", decimals_len, mantissa, abs(exponent));
    } else if (exponent <= -7) {
        decimals_len = double_get_decimal_length(mantissa, 8);
        printf("Result: %.*fe-%d\n", decimals_len, mantissa, abs(exponent));
    } else {
        printf("Result: %.*f\n", decimals_len, result);
    }

    return 0;
}
