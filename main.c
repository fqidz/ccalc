#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"


int main(void) {
    char input_string[256];
    TokenArr tokens = {0};

    printf("Input: \n");
    if (!fgets(input_string, sizeof(input_string) / sizeof(input_string[0]), stdin)) return 1;
    input_string[strcspn(input_string, "\n")] = 0;

    InputStream input_stream = {0};
    input_init(&input_stream, input_string);

    if (!input_tokenize(&tokens, &input_stream)) return 1;

    for (size_t i = 0; i < tokens.item_end_pos; i++) {
        printf("%zu:\t\"", i);
        char *current_token_name = tokens.items[i].value;
        for (size_t j = 0; j < strlen(current_token_name); j++) {
            printf("%c", current_token_name[j]);
        }
        printf("\"\n");
    }

    return 0;
}
