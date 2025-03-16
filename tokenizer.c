#include <ctype.h>
#include "tokenizer.h"
#include "logging.h"
#include <string.h>

int token_type_compare(TokenType lhs, TokenType rhs)
{
    switch (lhs) {
    case NUMBER:
        switch (rhs) {
        case NUMBER:
            return 0;
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            return -1;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
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
        case POWER:
            return -1;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
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
        case POWER:
            return -1;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
        }
    case POWER:
        switch (rhs) {
        case NUMBER:
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
            return 1;
        case POWER:
            return 0;
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
        }
    case LEFT_PAREN:
    case RIGHT_PAREN:
        switch (rhs) {
        case NUMBER:
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
        case LEFT_PAREN:
        case RIGHT_PAREN:
        default:
            UNREACHABLE;
        }
    default:
        UNREACHABLE;
    }
}

bool token_is_right_associative(TokenType token_type)
{
    switch (token_type) {
    case POWER:
        return true;
    case NUMBER:
    case PLUS:
    case MINUS:
    case MULTIPLY:
    case DIVIDE:
        return false;
    case LEFT_PAREN:
    case RIGHT_PAREN:
    default:
        UNREACHABLE;
    }
}

void tokenarr_init(TokenArr *tokenarr, size_t capacity)
{
    // tokenarr_free(tokenarr);
    tokenarr->capacity = capacity;
    tokenarr->length = 0;
    tokenarr->items = malloc(tokenarr->capacity * sizeof(Token));
}

void token_free(Token *token)
{
    free(token->value);
}

void tokenarr_append(TokenArr *tokenarr, Token item)
{
    if (!tokenarr->items) {
        tokenarr_init(tokenarr, 2);
    }

    if (tokenarr->length >= tokenarr->capacity) {
        tokenarr->capacity *= 2;
        tokenarr->items =
                realloc(tokenarr->items, tokenarr->capacity * sizeof(Token));
    }

    tokenarr->items[tokenarr->length++] = item;
}

Token tokenarr_pop(TokenArr *tokenarr)
{
    LOG_ASSERT(tokenarr->length > 0);
    return tokenarr->items[--tokenarr->length];
}

void tokenarr_free(TokenArr *tokenarr)
{
    for (size_t i = 0; i < tokenarr->length; i++) {
        token_free(&tokenarr->items[i]);
    }
    free(tokenarr->items);
    tokenarr->items = NULL;
    tokenarr->capacity = 0;
    tokenarr->length = 0;
}

char *tokenarr_to_string(TokenArr *tokenarr)
{
    char *string = calloc(2, sizeof(char));
    for (size_t i = 0; i < tokenarr->length; i++) {
        char *current_token_string = tokenarr->items[i].value;
        // add spaces between tokens
        if (i != 0) {
            char *new_token_string =
                    calloc(strlen(current_token_string) + 2, sizeof(char));
            strcpy(new_token_string, " ");
            strcat(new_token_string, current_token_string);
            string = realloc(string,
                             strlen(string) + strlen(new_token_string) + 1);
            strcat(string, new_token_string);
        } else {
            string = realloc(string,
                             strlen(string) + strlen(current_token_string) + 1);
            strcat(string, current_token_string);
        }
    }
    return string;
}

void string_remove_spaces(char *restrict str_trimmed,
                          char *restrict str_untrimmed)
{
    while (*str_untrimmed != '\0') {
        if (!isspace(*str_untrimmed)) {
            *str_trimmed = *str_untrimmed;
            str_trimmed++;
        }
        str_untrimmed++;
    }
    *str_trimmed = '\0';
}

void string_append_char(char **string, char c)
{
    char *c_as_string = calloc(2, sizeof(char));
    c_as_string[0] = c;
    LOG_ASSERT(c_as_string[1] == '\0');

    char *tmp = realloc(*string, (strlen(*string) + 2) * sizeof(char));
    if (!tmp) {
        UNREACHABLE;
    }

    strcat(tmp, c_as_string);
    *string = tmp;
}

void input_free(InputStream *input)
{
    free(input->string);
    input->length = 0;
    input->pos = 0;
}

void input_init(InputStream *input, char *string)
{
    LOG_ASSERT(strlen(string) > 0);
    char *trimmed_string = malloc(strlen(string) * sizeof(char));
    string_remove_spaces(trimmed_string, string);
    free(string);
    input->string = trimmed_string;
    input->length = strlen(trimmed_string);
    input->pos = 0;
}

bool input_is_eof(InputStream *input)
{
    LOG_ASSERT(input->pos <= input->length);
    return (input->pos == input->length);
}

char input_next(InputStream *input)
{
    LOG_ASSERT(!input_is_eof(input));
    return input->string[input->pos++];
}

char input_peek(InputStream *input)
{
    return input->string[input->pos];
}

bool is_symbol(char c)
{
    switch (c) {
    case '+':
    case '-':
    case '*':
    case 'x':
    case 'X':
    case '/':
    case '^':
        return true;
    default:
        return false;
    }
}

bool is_bracket(char c)
{
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

Error input_read_symbol(TokenArr *tokens, InputStream *input)
{
    LOG_ASSERT(!input_is_eof(input));
    LOG_ASSERT(is_symbol(input_peek(input)));
    char next_char = input_next(input);

    Token token = { 0 };
    token.value = calloc(2, sizeof(char));
    token.value[0] = next_char;
    token.pos = input->pos - 1;

    switch (next_char) {
    case '+':
        token.type = PLUS;
        break;
    case '-':
        token.type = MINUS;
        break;
    case '*':
    case 'x':
    case 'X':
        token.value[0] = '*';
        token.type = MULTIPLY;
        break;
    case '/':
        token.type = DIVIDE;
        break;
    case '^':
        token.type = POWER;
        break;
    default:
        UNREACHABLE;
    }

    tokenarr_append(tokens, token);
    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}

Error input_read_number(TokenArr *tokens, InputStream *input)
{
    LOG_ASSERT(!input_is_eof(input));
    LOG_ASSERT(isdigit(input_peek(input)) || input_peek(input) == '.');

    Token token = { 0 };
    token.value = calloc(2, sizeof(char));
    token.pos = input->pos;
    token.type = NUMBER;

    bool has_dot = false;
    while (!input_is_eof(input)) {
        char next_char = input_next(input);
        if (isdigit(next_char)) {
            string_append_char(&token.value, next_char);
        } else if (next_char == '.') {
            if (!has_dot) {
                string_append_char(&token.value, next_char);
                has_dot = true;
            } else {
                return (Error){
                    .type = NUMBER_EXTRA_DOT,
                    .input_string = input->string,
                    .char_pos = input->pos - 1,
                };
            }
        }

        if (!isdigit(input_peek(input)) && input_peek(input) != '.') {
            break;
        }
    }

    tokenarr_append(tokens, token);
    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}

Error input_read_bracket(TokenArr *tokens, InputStream *input)
{
    LOG_ASSERT(!input_is_eof(input));
    LOG_ASSERT(is_bracket(input_peek(input)));

    char next_char = input_next(input);

    Token token = { 0 };
    token.pos = input->pos - 1;
    token.value = calloc(2, sizeof(char));

    string_append_char(&token.value, next_char);

    switch (next_char) {
    case '(':
        token.type = LEFT_PAREN;
        break;
    case ')':
        token.type = RIGHT_PAREN;
        break;
    default:
        UNREACHABLE;
    }

    tokenarr_append(tokens, token);
    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}

Error input_tokenize(TokenArr *tokens, InputStream *input)
{
    while (!input_is_eof(input)) {
        char peeked_char = input_peek(input);
        Error token_error = { 0 };
        if (isdigit(peeked_char) || peeked_char == '.') {
            token_error = input_read_number(tokens, input);
        } else if (is_symbol(peeked_char)) {
            token_error = input_read_symbol(tokens, input);
        } else if (is_bracket(peeked_char)) {
            token_error = input_read_bracket(tokens, input);
        } else {
            return (Error){
                .type = INVALID_CHAR,
                .input_string = input->string,
                .char_pos = input->pos,
            };
        }

        if (token_error.type != NO_ERROR) {
            return token_error;
        }
    }

    return (Error){
        .type = NO_ERROR,
        .input_string = NULL,
        .char_pos = 0,
    };
}
