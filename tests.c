#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "calc.h"
#include "tests.h"

void test_string_remove_all_whitespace(void) {
    char *string = malloc(20 * sizeof(char));
    strcpy(string, " f o   o b    a r  ");
    char *trimmed_string = malloc(strlen(string) * sizeof(char));

    string_remove_spaces(trimmed_string, string);

    assert(strcmp(trimmed_string, "foobar") == 0);
}

void test_string_append_char(void) {
    char *string = malloc(9 * sizeof(char));

    strcpy(string, "chocobar");
    char c = 's';

    string_append_char(string, c);

    assert(strcmp(string, "chocobars") == 0);
}

void test_input_next(void) {
    const char *string = "test";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    assert(input_next(&input_stream) == 't');
    assert(input_next(&input_stream) == 'e');
    assert(input_next(&input_stream) == 's');
    assert(input_next(&input_stream) == 't');
}

void test_input_peek(void) {
    const char *string = "foobar";
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
    const char *string = "12345";
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
    const char *string = "b  a rt  ";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    assert(input_next(&input_stream) == 'b');
    assert(input_next(&input_stream) == 'a');
    assert(input_next(&input_stream) == 'r');
    assert(input_next(&input_stream) == 't');
    assert(input_is_eof(&input_stream));
}

void test_input_is_symbol(void) {
    const char *string = "+-*xX/";
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        assert(is_symbol(input_next(&input_stream)));
    }

    string = "x+-;/X*";
    input_free(&input_stream);
    input_init(&input_stream, string);
    assert(is_symbol(input_next(&input_stream)));
    assert(is_symbol(input_next(&input_stream)));
    assert(is_symbol(input_next(&input_stream)));
    assert(!is_symbol(input_next(&input_stream)));
    assert(is_symbol(input_next(&input_stream)));
    assert(is_symbol(input_next(&input_stream)));
    assert(is_symbol(input_next(&input_stream)));
}

void test_input_read_number(void) {
    char *string = malloc(11 * sizeof(char));
    strcpy(string, "1.02 + 232");
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    Token first_number = input_read_number(&input_stream);
    assert(strcmp(first_number.value, "1.02") == 0);
    assert(first_number.type == NUMBER);

    Token plus_symbol = input_read_symbol(&input_stream);
    assert(strcmp(plus_symbol.value, "+") == 0);
    assert(plus_symbol.type == PLUS);

    Token second_number = input_read_number(&input_stream);
    assert(strcmp(second_number.value, "232") == 0);
    assert(second_number.type == NUMBER);
}


void test_all(void) {
    test_string_remove_all_whitespace();
    test_string_append_char();
    test_input_next();
    test_input_peek();
    test_input_is_eof();
    test_input_with_whitespace();
    test_input_is_symbol();
    test_input_read_number();
}

int main(void) {
    test_all();
}
