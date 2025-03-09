#include <assert.h>
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
    size_t item_end_pos;
} TokenArr;

typedef struct {
    char *string;
    size_t length;
    size_t pos;
} InputStream;

void token_free(Token *token) {
    free(token->value);
}

void tokenarr_append(TokenArr *tokenarr, Token item) {
    if (!tokenarr->items) {
        tokenarr->length = 2;
        tokenarr->item_end_pos = 0;
        tokenarr->items = malloc(tokenarr->length * sizeof(Token));
    }

    if (tokenarr->item_end_pos >= tokenarr->length) {
        tokenarr->length *= 2;
        tokenarr->items = realloc(tokenarr->items, tokenarr->length * sizeof(Token));
    }

    tokenarr->items[tokenarr->item_end_pos++] = item;
}

void tokenarr_free(TokenArr *tokenarr) {
    for (int i = 0; i <= tokenarr->item_end_pos; i++) {
        token_free(&tokenarr->items[i]);
    }
    free(tokenarr->items);
    tokenarr->items = NULL;
    tokenarr->length = 0;
    tokenarr->item_end_pos = 0;
}

void string_remove_spaces (char* restrict str_trimmed, const char* restrict str_untrimmed)
{
  while (*str_untrimmed != '\0')
  {
    if(!isspace(*str_untrimmed))
    {
      *str_trimmed = *str_untrimmed;
      str_trimmed++;
    }
    str_untrimmed++;
  }
  *str_trimmed = '\0';
}

void input_free(InputStream *input) {
    free(input->string);
    input->length = 0;
    input->pos = 0;
}

void input_init(InputStream *input, const char *string) {
    char *trimmed_string = malloc(strlen(string) * sizeof(char));
    string_remove_spaces(trimmed_string, string);
    input->string = trimmed_string;
    input->length = strlen(trimmed_string);
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

Token input_read_symbol(InputStream *input) {
    Token token = {0};
    char current_char = input_next(input);
    token.value = malloc(2 * sizeof(char));
    switch (current_char) {
        case '+':
            token.type = PLUS;
        case '-':
            token.type = MINUS;
        default:
            exit(1);
    }
}


bool input_parse(TokenArr *tokens, InputStream *input) {

    return true;
}


void test_string_remove_all_whitespace(void) {
    char *string = " f o   o b    a r  ";
    char *trimmed_string = malloc(strlen(string) * sizeof(char));

    string_remove_spaces(trimmed_string, string);

    assert(strcmp(trimmed_string, "foobar") == 0);
}

void test_input_next(void) {
    char *string = "test";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    assert(input_next(&input_stream) == 't');
    assert(input_next(&input_stream) == 'e');
    assert(input_next(&input_stream) == 's');
    assert(input_next(&input_stream) == 't');
}

void test_input_peek(void) {
    char *string = "foobar";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    assert(input_peek(&input_stream) == 'f');
    assert(input_next(&input_stream) == 'f');

    assert(input_peek(&input_stream) == 'o');
    assert(input_next(&input_stream) == 'o');

    assert(input_peek(&input_stream) == 'o');
    assert(input_next(&input_stream) == 'o');

    assert(input_peek(&input_stream) == 'b');
    assert(input_next(&input_stream) == 'b');

    assert(input_peek(&input_stream) == 'a');
    assert(input_next(&input_stream) == 'a');

    assert(input_peek(&input_stream) == 'r');
    assert(input_next(&input_stream) == 'r');
}

void test_input_is_eof(void) {
    char *string = "12345";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);

    assert(input_is_eof(&input_stream));
}

void test_input_with_whitespace(void) {
    char *string = "b  a rt  ";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    assert(input_next(&input_stream) == 'b');
    assert(input_next(&input_stream) == 'a');
    assert(input_next(&input_stream) == 'r');
    assert(input_next(&input_stream) == 't');
    assert(input_is_eof(&input_stream));
}

bool test_all(void) {
    test_string_remove_all_whitespace();
    test_input_next();
    test_input_peek();
    test_input_is_eof();
    test_input_with_whitespace();
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
