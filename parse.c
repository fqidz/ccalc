#include "tokenizer.h"

void tokens_to_postfix(TokenArr *tokens) {
    // shunting yard algorithm
    TokenArr stack = {0};
    TokenArr output = {0};
    tokenarr_init(&output, tokens->length);
    for (size_t i = 0; i < tokens->item_count; i++) {
        Token current_token = tokens->items[i];
        switch (current_token.type) {
            case NUMBER:
                tokenarr_append(&output, current_token);
                break;
            case PLUS:
            case MINUS:
            case MULTIPLY:
            case DIVIDE:
                while (stack.item_count > 0) {
                    Token top_token_from_stack = stack.items[stack.item_count - 1];
                    int comparison = token_type_compare(current_token.type, top_token_from_stack.type);
                    if (comparison > 0) break;
                    tokenarr_append(&output, tokenarr_pop(&stack));
                }
                tokenarr_append(&stack, current_token);
                break;
            case LEFT_PAREN:
                tokenarr_append(&stack, current_token);
                break;
            case RIGHT_PAREN:
                while (stack.item_count > 0) {
                    Token top_token_from_stack = tokenarr_pop(&stack);
                    if (top_token_from_stack.type != LEFT_PAREN) {
                        tokenarr_append(&output, top_token_from_stack);
                    } else {
                        // discard matching left parenthesis
                        break;
                    }
                }
        }
        token_free(&current_token);
    }

}
