#include "tokenizer.h"

TokenArr tokens_to_postfix(TokenArr *tokens) {
    // shunting yard algorithm
    TokenArr output_tokens = {0};
    tokenarr_init(&output_tokens, tokens->length);
    for (size_t i = 0; i < tokens->item_end_pos; i++) {
        Token current_token = tokens->items[i];
        tokenarr_append(&output_tokens, current_token);
    }
    tokenarr_free(tokens);
    *tokens = output_tokens;
    tokenarr_free(&output_tokens);

}
