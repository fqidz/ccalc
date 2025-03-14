#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "logging.h"

char* new_string(const char* const string_literal) {
    LOG_ASSERT(strlen(string_literal) > 0);
    char *string = malloc((strlen(string_literal) + 1) * sizeof(char));
    strcpy(string, string_literal);

    return string;
}

// static void test_token_type_compare(void) {
//     //                         0   1 2 3 4 5 6    7 89 a b c d e f  g h i
//     char *string = new_string("1.2 + 3 / 7 * 10.2 - (2 + 3 - 5 * 8.2) * 3");
//
//     InputStream input_stream = {0};
//     TokenArr tokens = {0};
//     input_init(&input_stream, string);
//
//     input_tokenize(&tokens, &input_stream);
//
//     // find a better way to do this
//     for (size_t i = 0; i < tokens.item_count; i++) {
//         Token current_token = tokens.items[i];
//         switch (i) {
//             case 0: case 2: case 4: case 6: case 9: case 11: case 13: case 15: case 18:
//                 LOG_ASSERT(token_type_compare(NUMBER, current_token.type) == 0);
//                 break;
//             case 1: case 3: case 5: case 7: case 8: case 10: case 12: case 14: case 16: case 17:
//                 LOG_ASSERT(token_type_compare(NUMBER, current_token.type) == -1);
//                 break;
//             default:
//                 UNREACHABLE;
//         }
//     }
//
//     for (size_t i = 0; i < tokens.item_count; i++) {
//         Token current_token = tokens.items[i];
//         switch (i) {
//             case 0: case 2: case 4: case 6: case 9: case 11: case 13: case 15: case 18:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 1);
//                 break;
//             case 1: case 7: case 10: case 12:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 0);
//                 break;
//             case 3: case 5: case 8: case 14: case 16: case 17:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == -1);
//                 break;
//             default:
//                 UNREACHABLE;
//         }
//     }
//
//     for (size_t i = 0; i < tokens.item_count; i++) {
//         Token current_token = tokens.items[i];
//         switch (i) {
//             case 0: case 2: case 4: case 6: case 9: case 11: case 13: case 15: case 18:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 1);
//                 break;
//             case 1: case 7: case 10: case 12:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == 0);
//                 break;
//             case 3: case 5: case 8: case 14: case 16: case 17:
//                 LOG_ASSERT(token_type_compare(PLUS, current_token.type) == -1);
//                 break;
//             default:
//                 UNREACHABLE;
//         }
//     }
//
// }

static void test_input_tokenize(void) {
    char *string = new_string("16-12.0/2+(3*2+0.5+0.5)");

    TokenArr tokens = {0};
    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_tokenize(&tokens, &input_stream));
    for (size_t i = 0; i < tokens.length; i++) {
        Token token = tokens.items[i];
        switch (i) {
            case 0:
                LOG_ASSERT(strcmp(token.value, "16") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 1:
                LOG_ASSERT(strcmp(token.value, "-") == 0);
                LOG_ASSERT(token.type == MINUS);
                break;
            case 2:
                LOG_ASSERT(strcmp(token.value, "12.0") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 3:
                LOG_ASSERT(strcmp(token.value, "/") == 0);
                LOG_ASSERT(token.type == DIVIDE);
                break;
            case 4:
                LOG_ASSERT(strcmp(token.value, "2") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 5:
                LOG_ASSERT(strcmp(token.value, "+") == 0);
                LOG_ASSERT(token.type == PLUS);
                break;
            case 6:
                LOG_ASSERT(strcmp(token.value, "(") == 0);
                LOG_ASSERT(token.type == LEFT_PAREN);
                break;
            case 7:
                LOG_ASSERT(strcmp(token.value, "3") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 8:
                LOG_ASSERT(strcmp(token.value, "*") == 0);
                LOG_ASSERT(token.type == MULTIPLY);
                break;
            case 9:
                LOG_ASSERT(strcmp(token.value, "2") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 10:
                LOG_ASSERT(strcmp(token.value, "+") == 0);
                LOG_ASSERT(token.type == PLUS);
                break;
            case 11:
                LOG_ASSERT(strcmp(token.value, "0.5") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 12:
                LOG_ASSERT(strcmp(token.value, "+") == 0);
                LOG_ASSERT(token.type == PLUS);
                break;
            case 13:
                LOG_ASSERT(strcmp(token.value, "0.5") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 14:
                LOG_ASSERT(strcmp(token.value, ")") == 0);
                LOG_ASSERT(token.type == RIGHT_PAREN);
                break;
            default:
                UNREACHABLE;
        }
    }

    tokenarr_free(&tokens);
    string = new_string("2.2+44.4-3j*29/3");

    input_free(&input_stream);
    input_init(&input_stream, string);

    LOG_ASSERT(!input_tokenize(&tokens, &input_stream));

    LOG_ASSERT(tokens.length == 5);
    for (size_t i = 0; i < tokens.length; i++) {
        Token token = tokens.items[i];
        switch (i) {
            case 0:
                LOG_ASSERT(strcmp(token.value, "2.2") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 1:
                LOG_ASSERT(strcmp(token.value, "+") == 0);
                LOG_ASSERT(token.type == PLUS);
                break;
            case 2:
                LOG_ASSERT(strcmp(token.value, "44.4") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            case 3:
                LOG_ASSERT(strcmp(token.value, "-") == 0);
                LOG_ASSERT(token.type == MINUS);
                break;
            case 4:
                LOG_ASSERT(strcmp(token.value, "3") == 0);
                LOG_ASSERT(token.type == NUMBER);
                break;
            default:
                UNREACHABLE;
        }
    }
}

static void test_string_remove_all_whitespace(void) {
    char *string = new_string(" f o   o b    a r  ");

    char *trimmed_string = malloc(strlen(string) * sizeof(char));
    LOG_ASSERT(trimmed_string);

    string_remove_spaces(trimmed_string, string);

    LOG_ASSERT(strcmp(trimmed_string, "foobar") == 0);
}

static void test_string_append_char(void) {
    char *string = new_string("chocobar");

    string_append_char(string, 's');
    string_append_char(string, '.');
    string_append_char(string, 'c');
    string_append_char(string, 'o');
    string_append_char(string, 'm');

    LOG_ASSERT(strcmp(string, "chocobars.com") == 0);

    free(string);
    // empty string needs to be initialized with calloc instead of malloc
    string = calloc(2, sizeof(char));
    string_append_char(string, 'm');
    string_append_char(string, 'e');
    string_append_char(string, 'o');
    string_append_char(string, 'w');

    LOG_ASSERT(strcmp(string, "meow") == 0);
}

static void test_input_next(void) {
    char *string = new_string("test");

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_next(&input_stream) == 'e');
    LOG_ASSERT(input_next(&input_stream) == 's');
    LOG_ASSERT(input_next(&input_stream) == 't');
}

static void test_input_peek(void) {
    char *string = new_string("foobar");

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
    char *string = new_string("12345");

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
    char *string = new_string("b  a rt  ");

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 'b');
    LOG_ASSERT(input_next(&input_stream) == 'a');
    LOG_ASSERT(input_next(&input_stream) == 'r');
    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_is_eof(&input_stream));
}

static void test_input_read_number(void) {
    char *string = new_string("1.02 + 232 - .98");

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    Token first_number = input_read_number(&input_stream);
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
    char *string = new_string("+-*xX/");

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        LOG_ASSERT(is_symbol(input_next(&input_stream)));
    }

    string = new_string("x+-;/X*");
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
    char *string = new_string("())())))((())");

    InputStream input_stream = {0};
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        assert(is_bracket(input_next(&input_stream)));
    }

    string = new_string("ks:(;/)12");
    input_free(&input_stream);
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        char current_pos = input_stream.pos;
        char next_char = input_next(&input_stream);
        switch (current_pos) {
            case 0:
            case 1:
            case 2:
            case 4:
            case 5:
            case 7:
            case 8:
                LOG_ASSERT(!is_bracket(next_char));
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
    // test_token_type_compare();
    test_input_tokenize();

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
