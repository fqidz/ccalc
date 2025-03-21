#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../tokenizer.h"
#include "../logging.h"
#include "../parse.h"

static char *new_string(const char *const string_literal)
{
    LOG_ASSERT(strlen(string_literal) > 0);
    char *string = malloc((strlen(string_literal) + 1) * sizeof(char));
    strcpy(string, string_literal);

    return string;
}

static void test_double_format_to_string(void)
{
    double input;

    input = 0;
    LOG_ASSERT(strcmp(double_format_to_string(input), "0") == 0);

    input = 2.0000002;
    LOG_ASSERT(strcmp(double_format_to_string(input), "2.0000002") == 0);
    input = 108923019318731982371283719.0;
    LOG_ASSERT(strcmp(double_format_to_string(input), "1.089230193187e+26") ==
               0);
    input = 0.000000000000000000000007321;
    LOG_ASSERT(strcmp(double_format_to_string(input), "7.321e-24") == 0);

    input = -17;
    LOG_ASSERT(strcmp(double_format_to_string(input), "-17") == 0);
    input = -298329832913801283103.0;
    LOG_ASSERT(strcmp(double_format_to_string(input), "-2.983298329138e+20") ==
               0);
    input = -0.000000000000038238;
    LOG_ASSERT(strcmp(double_format_to_string(input), "-3.8238e-14") == 0);

    input = atof("1e2000");
    LOG_ASSERT(strcmp(double_format_to_string(input), "INF") == 0);
    input = atof("1e-2000");
    LOG_ASSERT(strcmp(double_format_to_string(input), "0") == 0);
}

static void test_tokens_to_postfix(void)
{
    char *string = new_string("2+3*9.0/(2-3*5^2)+.3^2");

    TokenArr tokens = { 0 };
    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    LOG_ASSERT(input_tokenize(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(tokens_to_postfix(&tokens, input_stream.string).type ==
               NO_ERROR);

    LOG_ASSERT(strcmp(tokenarr_to_string(&tokens),
                      "2 3 9.0 * 2 3 5 2 ^ * - / + .3 2 ^ +") == 0);

    free(string);
    string = new_string("(7.002+7-2.1*3/3.23/(300-2.33*(8+4)))"
                        "-(30*3)+(0.0-2^3^2^1^1+31-1^(300.3+300))");

    tokenarr_free(&tokens);
    input_free(&input_stream);
    input_init(&input_stream, string);

    LOG_ASSERT(input_tokenize(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(tokens_to_postfix(&tokens, input_stream.string).type ==
               NO_ERROR);

    LOG_ASSERT(strcmp(tokenarr_to_string(&tokens),
                      "7.002 7 + 2.1 3 * 3.23 / "
                      "300 2.33 8 4 + * - / - 30 "
                      "3 * - 0.0 2 3 2 1 1 ^ ^ ^ "
                      "^ - 31 + 1 300.3 300 + ^ - +") == 0);

    free(string);
    string = new_string("2+(.33+(2.1))+((2)-3398");

    tokenarr_free(&tokens);
    input_free(&input_stream);
    input_init(&input_stream, string);

    LOG_ASSERT(input_tokenize(&tokens, &input_stream).type == NO_ERROR);
    Error postfix_error = tokens_to_postfix(&tokens, input_stream.string);
    LOG_ASSERT(postfix_error.type == EXTRA_BRACKET);
    LOG_ASSERT(postfix_error.char_pos == 15);
    LOG_ASSERT(postfix_error.input_string);
}

static void test_evaluate_postfix_tokens(void)
{
    char *string = new_string("12+2*(3030/8/8/3*(3-2)-2)-2/4^10");

    TokenArr tokens = { 0 };
    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    double result = 0;
    LOG_ASSERT(input_tokenize(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(tokens_to_postfix(&tokens, input_stream.string).type ==
               NO_ERROR);
    printf("%s\n", tokenarr_to_string(&tokens));
    LOG_ASSERT(evaluate_postfix_tokens(&result, &tokens, input_stream.string)
                       .type == NO_ERROR);

    size_t needed = (size_t)snprintf(NULL, 0, "%.20f", result) + 1;
    char *result_as_string = malloc(needed);
    sprintf(result_as_string, "%.20f", result);

    LOG_ASSERT(strcmp(result_as_string, "39.56249809265136718750") == 0);
}

static void test_input_tokenize(void)
{
    char *string = new_string("16-12.0/2+(3*2+0.5+0.5)");

    TokenArr tokens = { 0 };
    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    Error tokenize_error = input_tokenize(&tokens, &input_stream);
    LOG_ASSERT(tokenize_error.type == NO_ERROR);

    LOG_ASSERT(strcmp(tokenarr_to_string(&tokens),
                      "16 - 12.0 / 2 + ( 3 * 2 + 0.5 + 0.5 )") == 0);

    tokenarr_free(&tokens);
    string = new_string("2.2+44.4-3j*29/3");

    input_free(&input_stream);
    input_init(&input_stream, string);

    tokenize_error = input_tokenize(&tokens, &input_stream);
    LOG_ASSERT(tokenize_error.type == INVALID_CHAR);
    LOG_ASSERT(tokenize_error.char_pos == 10);

    LOG_ASSERT(tokens.length == 5);
    LOG_ASSERT(strcmp(tokenarr_to_string(&tokens), "2.2 + 44.4 - 3") == 0);

    tokenarr_free(&tokens);
    string = new_string("3--.3+1.4*(-27/10*-33.333/-17-(1/2^-2))/1.1");

    input_free(&input_stream);
    input_init(&input_stream, string);

    tokenize_error = input_tokenize(&tokens, &input_stream);
    LOG_ASSERT(tokenize_error.type == NO_ERROR);

    LOG_ASSERT(strcmp(tokenarr_to_string(&tokens),
                      "3 - -.3 + 1.4 * ( -27 / 10 * -33.333 "
                      "/ -17 - ( 1 / 2 ^ -2 ) ) / 1.1") == 0);
}

static void test_string_remove_all_whitespace(void)
{
    char *string = new_string(" f o   o b    a r  ");

    char *trimmed_string = malloc(strlen(string) * sizeof(char));
    LOG_ASSERT(trimmed_string);

    string_remove_spaces(trimmed_string, string);

    LOG_ASSERT(strcmp(trimmed_string, "foobar") == 0);
}

static void test_string_append_char(void)
{
    char *string = new_string("chocobar");

    string_append_char(&string, 's');
    string_append_char(&string, '.');
    string_append_char(&string, 'c');
    string_append_char(&string, 'o');
    string_append_char(&string, 'm');

    LOG_ASSERT(strcmp(string, "chocobars.com") == 0);

    free(string);
    // empty string needs to be initialized with calloc instead of malloc
    string = calloc(2, sizeof(char));
    string_append_char(&string, 'm');
    string_append_char(&string, 'e');
    string_append_char(&string, 'o');
    string_append_char(&string, 'w');

    free(string);
    string = calloc(2, sizeof(char));
    char *chars =
            "to be or not to be, that is the question 123647580801&#*)(!@*#&%()";
    for (size_t i = 0; i < strlen(chars); i++) {
        string_append_char(&string, chars[i]);
    }

    LOG_ASSERT(
            strcmp(string,
                   "to be or not to be, that is the question 123647580801&#*)(!@*#&%()") ==
            0);
}

static void test_input_next(void)
{
    char *string = new_string("test");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_next(&input_stream) == 'e');
    LOG_ASSERT(input_next(&input_stream) == 's');
    LOG_ASSERT(input_next(&input_stream) == 't');
}

static void test_input_peek(void)
{
    char *string = new_string("foobar");

    InputStream input_stream = { 0 };
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

static void test_input_peek_nth(void)
{
    char *string = new_string("foobar");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    LOG_ASSERT(input_peek(&input_stream) == input_peek_nth(&input_stream, 1));
    LOG_ASSERT(input_peek_nth(&input_stream, 1) == 'f');
    LOG_ASSERT(input_peek_nth(&input_stream, 0) == '\0');
    LOG_ASSERT(input_peek_nth(&input_stream, -1) == '\0');
    LOG_ASSERT(input_peek_nth(&input_stream, -200) == '\0');
    LOG_ASSERT(input_peek_nth(&input_stream, 200) == '\0');

    LOG_ASSERT(input_next(&input_stream) == 'f');
    LOG_ASSERT(input_peek(&input_stream) == input_peek_nth(&input_stream, 1));
    LOG_ASSERT(input_peek_nth(&input_stream, -1) == '\0');
    LOG_ASSERT(input_peek_nth(&input_stream, 2) == 'o');
    LOG_ASSERT(input_peek_nth(&input_stream, 3) == 'b');
    LOG_ASSERT(input_peek_nth(&input_stream, 0) == 'f');
    LOG_ASSERT(input_peek_nth(&input_stream, 4) == 'a');
    LOG_ASSERT(input_peek_nth(&input_stream, 5) == 'r');
    LOG_ASSERT(input_peek_nth(&input_stream, 6) == '\0');

    LOG_ASSERT(input_next(&input_stream) == 'o');
    LOG_ASSERT(input_peek(&input_stream) == input_peek_nth(&input_stream, 1));
    LOG_ASSERT(input_peek_nth(&input_stream, 4) == 'r');
}

static void test_input_is_eof(void)
{
    char *string = new_string("12345");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    (void)input_next(&input_stream);
    (void)input_next(&input_stream);
    (void)input_next(&input_stream);
    (void)input_next(&input_stream);
    (void)input_next(&input_stream);

    LOG_ASSERT(input_is_eof(&input_stream));
}

static void test_input_with_whitespace(void)
{
    char *string = new_string("b  a rt  ");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    LOG_ASSERT(input_next(&input_stream) == 'b');
    LOG_ASSERT(input_next(&input_stream) == 'a');
    LOG_ASSERT(input_next(&input_stream) == 'r');
    LOG_ASSERT(input_next(&input_stream) == 't');
    LOG_ASSERT(input_is_eof(&input_stream));
}

static void test_input_read_number_symbol(void)
{
    char *string = new_string("1.02 + 232 - .98");

    InputStream input_stream = { 0 };
    TokenArr tokens;
    tokenarr_init(&tokens, 5);
    input_init(&input_stream, string);

    LOG_ASSERT(input_read_number(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(input_read_symbol(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(input_read_number(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(input_read_symbol(&tokens, &input_stream).type == NO_ERROR);
    LOG_ASSERT(input_read_number(&tokens, &input_stream).type == NO_ERROR);

    LOG_ASSERT(tokens.length == 5);

    LOG_ASSERT(strcmp(tokens.items[0].value, "1.02") == 0);
    LOG_ASSERT(tokens.items[0].type == NUMBER);

    LOG_ASSERT(strcmp(tokens.items[1].value, "+") == 0);
    LOG_ASSERT(tokens.items[1].type == PLUS);

    LOG_ASSERT(strcmp(tokens.items[2].value, "232") == 0);
    LOG_ASSERT(tokens.items[2].type == NUMBER);

    LOG_ASSERT(strcmp(tokens.items[3].value, "-") == 0);
    LOG_ASSERT(tokens.items[3].type == MINUS);

    LOG_ASSERT(strcmp(tokens.items[4].value, ".98") == 0);
    LOG_ASSERT(tokens.items[4].type == NUMBER);
}

static void test_input_is_operator(void)
{
    char *string = new_string("+-*xX/");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        LOG_ASSERT(is_operator(input_next(&input_stream)));
    }

    string = new_string("x+-;/X*");
    input_free(&input_stream);
    input_init(&input_stream, string);

    LOG_ASSERT(is_operator(input_next(&input_stream)));
    LOG_ASSERT(is_operator(input_next(&input_stream)));
    LOG_ASSERT(is_operator(input_next(&input_stream)));
    LOG_ASSERT(!is_operator(input_next(&input_stream)));
    LOG_ASSERT(is_operator(input_next(&input_stream)));
    LOG_ASSERT(is_operator(input_next(&input_stream)));
    LOG_ASSERT(is_operator(input_next(&input_stream)));
}

static void test_input_is_bracket(void)
{
    char *string = new_string("())())))((())");

    InputStream input_stream = { 0 };
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        assert(is_bracket(input_next(&input_stream)));
    }

    string = new_string("ks:(;/)12");
    input_free(&input_stream);
    input_init(&input_stream, string);

    while (!input_is_eof(&input_stream)) {
        size_t current_pos = input_stream.pos;
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

static void test_all(void)
{
    // test_token_type_compare();
    test_double_format_to_string();

    test_tokens_to_postfix();
    test_input_tokenize();
    test_evaluate_postfix_tokens();

    test_string_remove_all_whitespace();
    test_string_append_char();

    test_input_next();
    test_input_peek();
    test_input_peek_nth();
    test_input_is_eof();
    test_input_with_whitespace();
    test_input_read_number_symbol();

    test_input_is_operator();
    test_input_is_bracket();
}

int main(void)
{
    test_all();
}
