#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "logging.h"

static void test_token_type_compare(void) {
    //                  0   1 2 3 4 5 6    7 89 a b c d e f  g h i
    char *tmp_string = "1.2 + 3 / 7 * 10.2 - (2 + 3 - 5 * 8.2) * 3";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    TokenArr tokens = {0};
    input_init(&input_stream, string);
    input_tokenize(&tokens, &input_stream);

    // find a better way to do this
    for (size_t i = 0; i < tokens.item_count; i++) {
        Token current_token = tokens.items[i];
        switch (i) {
            case 0:
            case 2:
            case 4:
            case 6:
            case 9:
            case 11:
            case 13:
            case 15:
            case 18:
                LOG_ASSERT(token_type_compare(NUMBER, current_token.type) == 0);
                break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
            case 14:
            case 16:
            case 17:
                LOG_ASSERT(token_type_compare(NUMBER, current_token.type) == -1);
                break;
            default:
                UNREACHABLE;
        }
    }

    for (size_t i = 0; i < tokens.item_count; i++) {
        Token current_token = tokens.items[i];
        switch (i) {
            case 0:
            case 2:
            case 4:
            case 6:
            case 9:
            case 11:
            case 13:
            case 15:
            case 18:
                LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 1);
                break;
            case 1:
            case 7:
            case 10:
            case 12:
                LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 0);
                break;
            case 3:
            case 5:
            case 8:
            case 14:
            case 16:
            case 17:
                LOG_ASSERT(token_type_compare(PLUS, current_token.type) == -1);
                break;
            default:
                UNREACHABLE;
        }
    }

}

static void test_string_remove_all_whitespace(void) {
    char *tmp_string = " f o   o b    a r  ";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    char *trimmed_string = malloc(strlen(string) * sizeof(char));
    LOG_ASSERT(trimmed_string);

    string_remove_spaces(trimmed_string, string);

    LOG_ASSERT(strcmp(trimmed_string, "foobar") == 0);
}

static void test_string_append_char(void) {
    char *tmp_string = "chocobar";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    LOG_ASSERT(string);
    strcpy(string, "chocobar");
    char c = 's';

    string_append_char(string, c);

    LOG_ASSERT(strcmp(string, "chocobars") == 0);
}

static void test_input_next(void) {
    char *tmp_string = "test";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_next(&input_stream) == 'e');
    LOG_ASSERT(input_next(&input_stream) == 's');
    LOG_ASSERT(input_next(&input_stream) == 't');
}

static void test_input_peek(void) {
    char *tmp_string = "foobar";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_peek(&input_stream) == 'f');
    LOG_ASSERT(input_next(&input_stream) == 'f');

    LOG_ASSERT(input_peek(&input_stream) == 'o');
    LOG_ASSERT(input_next(&input_stream) == 'o');

    LOG_ASSERT(input_peek(&input_stream) == 'o');
    LOG_ASSERT(input_next(&input_stream) == 'o');

    LOG_ASSERT(input_peek(&input_stream) == 'b');
    LOG_ASSERT(input_next(&input_stream) == 'b');

    LOG_ASSERT(input_peek(&input_stream) == 'a');
    LOG_ASSERT(input_next(&input_stream) == 'a');

    LOG_ASSERT(input_peek(&input_stream) == 'r');
    LOG_ASSERT(input_next(&input_stream) == 'r');
}

static void test_input_is_eof(void) {
    char *tmp_string = "12345";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);
    (void) input_next(&input_stream);

    LOG_ASSERT(input_is_eof(&input_stream));
}

static void test_input_with_whitespace(void) {
    char *tmp_string = "b  a rt  ";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 'b');
    LOG_ASSERT(input_next(&input_stream) == 'a');
    LOG_ASSERT(input_next(&input_stream) == 'r');
    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_is_eof(&input_stream));
}

static void test_input_read_number(void) {
    char *tmp_string = "1.02 + 232 - .98";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    Token first_number = input_read_number(&input_stream);
    printf("%s\n", first_number.value);
    printf("%s\n", input_stream.string);
    printf("%i\n", strcmp(first_number.value, "1.02"));

    LOG_ASSERT(strcmp(first_number.value, "1.02") == 0);
    LOG_ASSERT(first_number.type == NUMBER);

    Token plus_symbol = input_read_symbol(&input_stream);
    LOG_ASSERT(strcmp(plus_symbol.value, "+") == 0);
    LOG_ASSERT(plus_symbol.type == PLUS);

    Token second_number = input_read_number(&input_stream);
    LOG_ASSERT(strcmp(second_number.value, "232") == 0);
    LOG_ASSERT(second_number.type == NUMBER);

    Token minus_symbol = input_read_symbol(&input_stream);
    LOG_ASSERT(strcmp(minus_symbol.value, "-") == 0);
    LOG_ASSERT(minus_symbol.type == MINUS);

    Token third_number = input_read_number(&input_stream);
    LOG_ASSERT(strcmp(third_number.value, ".98") == 0);
    LOG_ASSERT(third_number.type == NUMBER);
}

static void test_input_is_symbol(void) {
    char *tmp_string = "+-*xX/";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        LOG_ASSERT(is_symbol(input_next(&input_stream)));
    }

    free(string);
    tmp_string = "x+-;/X*";
    string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    input_free(&input_stream);
    input_init(&input_stream, string);
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
    LOG_ASSERT(!is_symbol(input_next(&input_stream)));
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
    LOG_ASSERT(is_symbol(input_next(&input_stream)));
}

static void test_input_is_bracket(void) {
    char *tmp_string = "())())))((())";
    char *string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        assert(is_bracket(input_next(&input_stream)));
    }

    free(string);
    tmp_string = "ks:(;/)12";
    string = malloc((strlen(tmp_string) + 1) * sizeof(char));
    strcpy(string, tmp_string);

    input_free(&input_stream);
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        char next_char = input_next(&input_stream);
        switch (next_char) {
            case 0:
            case 1:
            case 2:
            case 4:
            case 5:
            case 7:
            case 8:
                LOG_ASSERT(is_bracket(next_char));
                break;
            case 3:
            case 6:
                LOG_ASSERT(is_bracket(next_char));
                break;
            default:
                UNREACHABLE;
        }
    }
}


static void test_all(void) {
    test_token_type_compare();

    test_string_remove_all_whitespace();
    test_string_append_char();

    test_input_next();
    test_input_peek();
    test_input_is_eof();
    test_input_with_whitespace();
    test_input_read_number();

    test_input_is_symbol();
    test_input_is_bracket();
}

int main(void) {
    test_all();
}
