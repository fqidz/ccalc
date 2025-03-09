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

void string_append_char(char *string, char c) {
    char c_as_string[2];
    c_as_string[0] = c;
    c_as_string[1] = '\0';

    char *tmp = realloc(string, (strlen(string) + 3) * sizeof(char));
    if (!tmp) exit(1);

    string = tmp;
    strcat(string, c_as_string);
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

bool input_is_eof(InputStream *input) {
    assert(input->pos <= input->length);
    return (input->pos == input->length);
}

char input_next(InputStream *input) {
    assert(!input_is_eof(input));
    return input->string[input->pos++];
}

char input_peek(InputStream *input) {
    // assert(!input_is_eof(input));
    return input->string[input->pos];
}

bool is_symbol(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case 'x':
        case 'X':
        case '/':
            return true;
        default:
            return false;
    }
}

Token input_read_symbol(InputStream *input) {
    assert(!input_is_eof(input));
    assert(is_symbol(input_peek(input)));
    Token token = {0};
    switch (input_next(input)) {
        case '+':
            // might be undefined behaviour cause its a string literal?
            token.value = "+";
            token.type = PLUS;
            break;
        case '-':
            token.value = "-";
            token.type = MINUS;
            break;
        case '*':
        case 'x':
        case 'X':
            token.value = "*";
            token.type = MULTIPLY;
            break;
        case '/':
            token.value = "/";
            token.type = DIVIDE;
            break;
        default:
            // unreachable
            exit(1);
    }

    return token;
}

Token input_read_number(InputStream *input) {
    assert(!input_is_eof(input));
    assert(isdigit(input_peek(input)) || input_peek(input) == '.');

    Token token = {0};
    token.value = malloc(2 * sizeof(char));
    token.type = NUMBER;

    bool has_dot = false;
    while (!input_is_eof(input)) {
        char next_char = input_next(input);
        if (isdigit(next_char)) {
            string_append_char(token.value, next_char);
        } else if (next_char == '.') {
            if (!has_dot) {
                string_append_char(token.value, next_char);
                has_dot = true;
            } else {
                printf("%c\n", next_char);
                // TODO: error
                exit(1);
            }
        }

        if (!isdigit(input_peek(input)) && input_peek(input) != '.') {
            break;
        }
    }

    return token;
}

bool parse(TokenArr *tokens, InputStream *input) {

    return true;
}

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
