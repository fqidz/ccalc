#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "calc.h"


int main(void) {
    char input_string[20];

    printf("Input: ");
    if (!fgets(input_string, sizeof(input_string) / sizeof(input_string[0]), stdin)) return 1;
    input_string[strcspn(input_string, "\n")] = 0;

    InputStream input_stream = {0};
    input_init(&input_stream, input_string);

    while (!input_is_eof(&input_stream)) {
        char next_char = input_next(&input_stream);
        printf("Pos %zu: '%c'\n", input_stream.pos, next_char);
    }

    return 0;
}
