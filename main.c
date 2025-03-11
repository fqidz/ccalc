#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "calc.h"


int main(void) {
    char input_string[256];

    printf("Input: \n");
    if (!fgets(input_string, sizeof(input_string) / sizeof(input_string[0]), stdin)) return 1;
    input_string[strcspn(input_string, "\n")] = 0;

    InputStream input_stream = {0};
    input_init(&input_stream, input_string);

    printf("--------------\n");
    while (!input_is_eof(&input_stream)) {
        size_t current_pos = input_stream.pos;
        char next_char = input_next(&input_stream);
        printf("Pos %zu:\t'%c'\n", current_pos, next_char);
    }

    return 0;
}
