#include "logging.h"
#include "tokenizer.h"

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
