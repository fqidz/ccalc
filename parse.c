#include "logging.h"
#include "tokenizer.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

Error tokens_to_postfix(TokenArr *tokens, char *input_string)
{
    LOG_ASSERT(tokens->length > 0);
    // shunting yard algorithm
    TokenArr stack = { 0 };
    TokenArr output = { 0 };
    size_t last_left_paren_pos = 0;
    int brackets_count = 0;
    tokenarr_init(&output, tokens->capacity);
    for (size_t i = 0; i < tokens->length; i++) {
        Token current_token = tokens->items[i];

        switch (current_token.type) {
        case NUMBER:
            tokenarr_append(&output, current_token);
            break;
        case NEGATIVE:
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            while (stack.length > 0) {
                Token last_stack_token = stack.items[stack.length - 1];
                if (last_stack_token.type == LEFT_PAREN)
                    break;
                int comparison = token_type_compare_precidence(
                        current_token.type, last_stack_token.type);
                if (comparison == 1 ||
                    token_is_right_associative(current_token.type))
                    break;
                tokenarr_append(&output, tokenarr_pop(&stack));
            }
            tokenarr_append(&stack, current_token);
            break;
        case LEFT_PAREN:
            brackets_count++;
            last_left_paren_pos = current_token.pos;
            tokenarr_append(&stack, current_token);
            break;
        case RIGHT_PAREN:
            brackets_count--;
            if (brackets_count < 0) {
                return (Error){
                    .type = EXTRA_BRACKET,
                    .char_pos = current_token.pos,
                    .input_string = input_string,
                };
            }
            while (stack.length > 0) {
                Token popped_stack_token = tokenarr_pop(&stack);
                if (popped_stack_token.type != LEFT_PAREN) {
                    tokenarr_append(&output, popped_stack_token);
                } else {
                    break;
                }
            }
            break;
        default:
            UNREACHABLE;
        }
    }

    if (brackets_count != 0) {
        return (Error){
            .type = EXTRA_BRACKET,
            .char_pos = last_left_paren_pos,
            .input_string = input_string,
        };
    }

    while (stack.length > 0) {
        tokenarr_append(&output, tokenarr_pop(&stack));
    }

    *tokens = output;

    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}

Error evaluate_postfix_tokens(double *result, TokenArr *tokens,
                              char *input_string)
{
    LOG_ASSERT(tokens->length > 0);
    double *stack = calloc(tokens->length, sizeof(double));
    size_t stack_length = 0;
    char *endptr;

    for (size_t i = 0; i < tokens->length; i++) {
        Token token = tokens->items[i];
        double operation_result = 0.0;
        double string_as_double = 0.0;
        switch (token.type) {
        case NUMBER:
            string_as_double = strtod(token.value, &endptr);
            LOG_ASSERT(strlen(endptr) == 0);
            stack[stack_length] = string_as_double;
            stack_length++;
            break;
        case NEGATIVE:
            stack[stack_length - 1] *= -1;
            break;
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            if (stack_length < 2) {
                return (Error){
                    .type = EXTRA_OPERATOR,
                    .char_pos = token.pos,
                    .input_string = input_string,
                };
            }
            double lhs = stack[stack_length - 2];
            double rhs = stack[stack_length - 1];
            stack_length -= 2;

            switch (token.type) {
            case PLUS:
                operation_result = lhs + rhs;
                break;
            case MINUS:
                operation_result = lhs - rhs;
                break;
            case MULTIPLY:
                operation_result = lhs * rhs;
                break;
            case DIVIDE:
                // TODO: handle division by zero
                operation_result = lhs / rhs;
                break;
            case POWER:
                operation_result = pow(lhs, rhs);
                break;
            case NUMBER:
            case NEGATIVE:
            case LEFT_PAREN:
            case RIGHT_PAREN:
            default:
                UNREACHABLE;
            }

            stack[stack_length] = operation_result;
            stack_length++;
            break;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
        }
    }

    LOG_ASSERT(stack_length == 1);

    *result = stack[0];
    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}

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

    free(buffer);

    LOG_ASSERT(first_non_zero_index > 0);
    return (int)first_non_zero_index - 1;
}

double double_get_scientific_notation(double input, int *exponent)
{
    *exponent = (int)(floor(log10(fabs(input))));
    return input / pow(10, *exponent);
}

char *double_format_to_string(double input)
{
    if (input == HUGE_VAL) {
        char *string = malloc(4 * sizeof(char));
        strcpy(string, "INF");
        return string;
    } else if (fabs(input) <= DBL_MIN) {
        char *string = malloc(2 * sizeof(char));
        strcpy(string, "0");
        return string;
    }

    size_t needed_bytes;
    char *buffer;
    int decimals_len;
    int exponent;
    double mantissa = double_get_scientific_notation(input, &exponent);

    if (exponent >= 12) {
        decimals_len = double_get_decimal_length(mantissa, 12);

        needed_bytes = (size_t)snprintf(NULL, 0, "%.*fe+%d", decimals_len,
                                        mantissa, abs(exponent)) +
                       1;
        buffer = malloc(needed_bytes);
        sprintf(buffer, "%.*fe+%d", decimals_len, mantissa, abs(exponent));
    } else if (exponent <= -7) {
        decimals_len = double_get_decimal_length(mantissa, 8);
        needed_bytes = (size_t)snprintf(NULL, 0, "%.*fe-%d", decimals_len,
                                        mantissa, abs(exponent) + 1) +
                       1;
        buffer = malloc(needed_bytes);
        sprintf(buffer, "%.*fe-%d", decimals_len, mantissa, abs(exponent));
    } else {
        decimals_len = double_get_decimal_length(input, 12);
        needed_bytes =
                (size_t)snprintf(NULL, 0, "%.*f", decimals_len, input + 1) + 1;
        buffer = malloc(needed_bytes);
        sprintf(buffer, "%.*f", decimals_len, input);
    }

    return buffer;
}
