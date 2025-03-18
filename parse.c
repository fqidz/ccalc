#include "logging.h"
#include "tokenizer.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>

Error tokens_to_postfix(TokenArr *tokens, char *input_string)
{
    LOG_ASSERT(tokens->length > 0);
    // shunting yard algorithm
    TokenArr stack = { 0 };
    TokenArr output = { 0 };
    int brackets_count = 0;
    tokenarr_init(&output, tokens->capacity);
    for (size_t i = 0; i < tokens->length; i++) {
        Token current_token = tokens->items[i];

        switch (current_token.type) {
        case NUMBER:
            tokenarr_append(&output, current_token);
            break;
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            while (stack.length > 0) {
                Token last_stack_token = stack.items[stack.length - 1];
                if (last_stack_token.type == LEFT_PAREN)
                    break;
                int comparison = token_type_compare(current_token.type,
                                                    last_stack_token.type);
                if (comparison == 1 ||
                    token_is_right_associative(current_token.type))
                    break;
                tokenarr_append(&output, tokenarr_pop(&stack));
            }
            tokenarr_append(&stack, current_token);
            break;
        case LEFT_PAREN:
            brackets_count++;
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

Error evaluate_postfix_tokens(double *result, TokenArr *tokens)
{
    LOG_ASSERT(tokens->length > 0);
    double *stack = calloc(tokens->length, sizeof(double));
    int stack_length = 0;
    char *endptr;

    for (size_t i = 0; i < tokens->length; i++) {
        Token token = tokens->items[i];
        double operation_result = 0.0;
        double string_as_double = 0.0;
        switch (token.type) {
        case NUMBER:
            string_as_double = strtod(token.value, &endptr);
            // TODO: handle error
            // printf("num: %.15f\n", string_as_double);
            if (string_as_double > DBL_MAX) {
                return (Error){
                    .type = NUMBER_TOO_LARGE,
                    .char_pos = token.pos,
                    .input_string = "",
                };
            }
            LOG_ASSERT(strlen(endptr) == 0);
            stack[stack_length] = string_as_double;
            stack_length++;
            break;
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            if (stack_length < 2) {
                UNREACHABLE;
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
