#include "tokenizer.h"

void tokens_to_postfix(TokenArr *tokens) {
    // shunting yard algorithm
    TokenArr stack = {0};
    size_t j = 0;
    for (size_t i = 0; i < tokens->item_end_pos; i++) {
        Token current_token = tokens->items[i];
        if (current_token.type == NUMBER) {
            tokens->items[j] = current_token;
            j++;
        } else if (
            current_token.type == PLUS ||
            current_token.type == MINUS ||
            current_token.type == MULTIPLY ||
            current_token.type == DIVIDE ||
            current_token.type == LEFT_PAREN ||
            current_token.type == RIGHT_PAREN
        ) {
            tokenarr_append(&stack, current_token);
        }
        token_free(&current_token);
    }

}
