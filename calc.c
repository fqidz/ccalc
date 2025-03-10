#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "calc.h"

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
    for (size_t i = 0; i <= tokenarr->item_end_pos; i++) {
        token_free(&tokenarr->items[i]);
    }
    free(tokenarr->items);
    tokenarr->items = NULL;
    tokenarr->length = 0;
    tokenarr->item_end_pos = 0;
}

void string_remove_spaces (char* restrict str_trimmed, const char* restrict str_untrimmed) {
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

// bool parse(TokenArr *tokens, InputStream *input) {
//
//     return true;
// }
