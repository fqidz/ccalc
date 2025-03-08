#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LEFT_PAREN,
    RIGHT_PAREN,
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

typedef struct {
    Token *items;
    size_t length;
    size_t pos;
} TokenArr;

void tokenarr_append(TokenArr *tokenarr, Token item) {
    if (!tokenarr->items) {
        tokenarr->length = 2;
        tokenarr->pos = 0;
        tokenarr->items = malloc(tokenarr->length * sizeof(Token));
    }

    if (tokenarr->pos >= tokenarr->length) {
        tokenarr->length *= 2;
        tokenarr->items = realloc(tokenarr->items, tokenarr->length * sizeof(Token));
    }

    tokenarr->items[tokenarr->pos++] = item;
}

void tokenarr_free(TokenArr *tokenarr) {
    free(tokenarr->items);
    tokenarr->items = NULL;
    tokenarr->length = 0;
    tokenarr-> pos = 0;
}

typedef struct {
    char *string;
    size_t length;
    size_t pos;
} InputStream;

void string_remove_all_whitespace(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

void input_init(InputStream *input, char *string) {
    string_remove_all_whitespace(string);
    input->string = string;
    input->length = strlen(string);
    input->pos = 0;
}

char input_next(InputStream *input) {
    return input->string[input->pos++];
}

char input_peek(InputStream *input) {
    return input->string[input->pos];
}

bool input_is_eof(InputStream *input) {
    return (input->pos >= input->length);
}

// Token input_read_symbol(InputStream *input) {
//
// }


bool input_parse(TokenArr *tokens, InputStream *input) {

    return true;
}


bool test_string_remove_all_whitespace() {
    char *string = " f o   o b    a r  ";
    int original_len = strlen(string);
    string_remove_all_whitespace(string);
    if (strcmp(string, "foobar") != 0) return false;
    if (original_len != strlen(string)) return false;
    return true;
}

bool test_input_next(void) {
    char *string = "test";
    InputStream input_stream = {0};
    input_init(&input_stream, string);
    if (input_next(&input_stream) != 't') return false;
    if (input_next(&input_stream) != 'e') return false;
    if (input_next(&input_stream) != 's') return false;
    if (input_next(&input_stream) != 't') return false;
    return true;
}

bool test_input_peek(void) {
    char *string = "foo bar";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    if (input_peek(&input_stream) != 'f') return false;
    if (input_next(&input_stream) != 'f') return false;

    if (input_peek(&input_stream) != 'o') return false;
    if (input_next(&input_stream) != 'o') return false;

    if (input_peek(&input_stream) != 'o') return false;
    if (input_next(&input_stream) != 'o') return false;

    if (input_peek(&input_stream) != ' ') return false;
    if (input_next(&input_stream) != ' ') return false;

    if (input_peek(&input_stream) != 'b') return false;
    if (input_next(&input_stream) != 'b') return false;

    if (input_peek(&input_stream) != 'a') return false;
    if (input_next(&input_stream) != 'a') return false;

    if (input_peek(&input_stream) != 'r') return false;
    if (input_next(&input_stream) != 'r') return false;

    return true;
}

bool test_input_is_eof(void) {
    char *string = "12345";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);

    if (!input_is_eof(&input_stream)) return false;

    return true;
}

bool test_input_skip_whitespace(void) {
    char *string = "b  a rt  ";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    // if (input_next(&input_stream) != 'b') return false;
    // input_skip_whitespaces(&input_stream);
    // input_skip_whitespaces(&input_stream);
    // if (input_next(&input_stream) != 'a') return false;
    // input_skip_whitespaces(&input_stream);
    // if (input_next(&input_stream) != 'r') return false;
    // if (input_next(&input_stream) != 't') return false;
    // if (input_is_eof(&input_stream)) return false;
    // input_skip_whitespaces(&input_stream);
    // if (!input_is_eof(&input_stream)) return false;
    return true;
}

bool test_all(void) {
    if (!test_string_remove_all_whitespace()) return false;
    if (!test_input_next()) return false;
    if (!test_input_peek()) return false;
    if (!test_input_is_eof()) return false;
    if (!test_input_skip_whitespace()) return false;
    return true;
}

int main(void) {
    if (!test_all()) {
        printf("Tests Failed");
        return 1;
    }

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
