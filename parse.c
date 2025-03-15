#include "logging.h"
#include "tokenizer.h"
#include <math.h>
#include <stdlib.h>

void tokens_to_postfix(TokenArr *tokens)
{
    LOG_ASSERT(tokens->length > 0);
    // shunting yard algorithm
    TokenArr stack = { 0 };
    TokenArr output = { 0 };
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
            tokenarr_append(&stack, current_token);
            break;
        case RIGHT_PAREN:
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
}

double evaluate_postfix_tokens(TokenArr *tokens)
{
    LOG_ASSERT(tokens->length > 0);
    double *stack;
    stack = calloc(tokens->length, sizeof(double));
    int stack_length = 0;
    char *endptr;

    for (size_t i = 0; i < tokens->length; i++) {
        Token token = tokens->items[i];
        double result = 0.0;
        switch (token.type) {
        case NUMBER:
            stack[stack_length] = strtod(token.value, &endptr);
            LOG_ASSERT(strlen(endptr) == 0);
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
                result = lhs + rhs;
                break;
            case MINUS:
                result = lhs - rhs;
                break;
            case MULTIPLY:
                result = lhs * rhs;
                break;
            case DIVIDE:
                result = lhs / rhs;
                break;
            case POWER:
                result = pow(lhs, rhs);
                break;
            case NUMBER:
            case LEFT_PAREN:
            case RIGHT_PAREN:
            default:
                UNREACHABLE;
            }

            stack[stack_length] = result;
            stack_length++;
            break;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
        }
    }

    LOG_ASSERT(stack_length == 1);

    return stack[0];
}
