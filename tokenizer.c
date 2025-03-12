#include <ctype.h>
#include "tokenizer.h"

int token_type_compare(TokenType lhs, TokenType rhs) {
    switch (lhs) {
        case NUMBER:
            switch (rhs) {
                case NUMBER:
                    return 0;
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                case LEFT_PAREN:
                case RIGHT_PAREN:
                    return -1;
            }
        case PLUS:
        case MINUS:
            switch (rhs) {
                case NUMBER:
                    return 1;
                case PLUS:
                case MINUS:
                    return 0;
                case MULTIPLY:
                case DIVIDE:
                case LEFT_PAREN:
                case RIGHT_PAREN:
                    return -1;
            }
        case MULTIPLY:
        case DIVIDE:
            switch (rhs) {
                case NUMBER:
                case PLUS:
                case MINUS:
                    return 1;
                case MULTIPLY:
                case DIVIDE:
                    return 0;
                case LEFT_PAREN:
                case RIGHT_PAREN:
                    return -1;
            }
        case LEFT_PAREN:
        case RIGHT_PAREN:
            switch (rhs) {
                case NUMBER:
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                    return 1;
                case LEFT_PAREN:
                case RIGHT_PAREN:
                    return 0;
            }
    }
}

void tokenarr_init(TokenArr *tokenarr, size_t length) {
    // tokenarr_free(tokenarr);
    tokenarr->length = length;
    tokenarr->item_count = 0;
    tokenarr->items = malloc(tokenarr->length * sizeof(Token));
}

void token_free(Token *token) {
    free(token->value);
}

void tokenarr_append(TokenArr *tokenarr, Token item) {
    if (!tokenarr->items) {
        tokenarr_init(tokenarr, 2);
    }

    if (tokenarr->item_count >= tokenarr->length) {
        tokenarr->length *= 2;
        tokenarr->items = realloc(tokenarr->items, tokenarr->length * sizeof(Token));
    }

    tokenarr->items[tokenarr->item_count++] = item;
}

Token tokenarr_pop(TokenArr *tokenarr) {
    assert(tokenarr->item_count > 0);
    return tokenarr->items[--tokenarr->item_count];
}

void tokenarr_free(TokenArr *tokenarr) {
    for (size_t i = 0; i <= tokenarr->item_count; i++) {
        token_free(&tokenarr->items[i]);
    }
    free(tokenarr->items);
    tokenarr->items = NULL;
    tokenarr->length = 0;
    tokenarr->item_count = 0;
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
    char *c_as_string = malloc(2 * sizeof(char));
    c_as_string[0] = c;
    c_as_string[1] = '\0';

    char *tmp = realloc(string, (strlen(string) + strlen(c_as_string) + 1) * sizeof(char));
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

bool is_bracket(char c) {
    switch (c) {
        case '(':
        case ')':
        // case '[':
        // case ']':
        // case '{':
        // case '}':
            return true;
        default:
            return false;
    }
}

Token input_read_symbol(InputStream *input) {
    assert(!input_is_eof(input));
    assert(is_symbol(input_peek(input)));
    Token token = {0};
    token.value = malloc(2 * sizeof(char));
    token.value[1] = '\0';

    switch (input_next(input)) {
        case '+':
            // might be undefined behaviour cause its a string literal?
            token.value[0] = '+';
            token.type = PLUS;
            break;
        case '-':
            token.value[0] = '-';
            token.type = MINUS;
            break;
        case '*':
        case 'x':
        case 'X':
            token.value[0] = '*';
            token.type = MULTIPLY;
            break;
        case '/':
            token.value[0] = '/';
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

Token input_read_bracket(InputStream *input) {
    assert(!input_is_eof(input));
    assert(is_bracket(input_peek(input)));

    Token token = {0};
    char next_char = input_next(input);

    token.value = malloc(2 * sizeof(char));
    string_append_char(token.value, next_char);

    switch (next_char) {
        case '(':
            token.type = LEFT_PAREN;
            break;
        case ')':
            token.type = RIGHT_PAREN;
            break;
        default:
            // unreachable
            exit(1);
    }

    return token;
}

bool input_tokenize(TokenArr *tokens, InputStream *input) {
    while (!input_is_eof(input)) {
        char peeked_char = input_peek(input);
        Token token = {0};
        if (isdigit(peeked_char) || peeked_char == '.') {
            token = input_read_number(input);
        } else if (is_symbol(peeked_char)) {
            token = input_read_symbol(input);
        } else if (is_bracket(peeked_char)) {
            token = input_read_bracket(input);
        } else {
            printf("[ERROR] %zu: Invalid character '%c'\n", input->pos, peeked_char);
            return false;
        }

        tokenarr_append(tokens, token);
    }

    return true;
}
