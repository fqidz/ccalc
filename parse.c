#include "logging.h"
#include "tokenizer.h"
#include <stdlib.h>

void tokens_to_postfix(TokenArr *tokens)
{
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
            while (stack.length > 0) {
                Token last_stack_token = stack.items[stack.length - 1];
                if (last_stack_token.type == LEFT_PAREN)
                    break;
                int comparison = token_type_compare(current_token.type,
                                                    last_stack_token.type);
                if (comparison == 1)
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
        case DIVIDE:;
            if (stack_length < 2) {
                UNREACHABLE;
            }
            double lhs = stack[stack_length - 2];
            double rhs = stack[stack_length - 1];
            stack_length -= 2;

            if (token.type == PLUS) {
                result = lhs + rhs;
            } else if (token.type == MINUS) {
                result = lhs - rhs;
            } else if (token.type == MULTIPLY) {
                result = lhs * rhs;
            } else if (token.type == DIVIDE) {
                result = lhs / rhs;
            } else {
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
