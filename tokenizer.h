#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "logging.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// ---------------------------------------------------------

typedef enum {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    LEFT_PAREN,
    RIGHT_PAREN,
} TokenType;

/*
 * Return:
 * '-1' lhs < rhs
 *
 * '0' lhs == rhs
 *
 * '1' lhs > rhs
 * */
int token_type_compare(TokenType lhs, TokenType rhs);
bool token_is_right_associative(TokenType token_type);

// ---------------------------------------------------------

typedef struct {
    char *value;
    size_t pos;
    TokenType type;
} Token;

void token_free(Token *token);

// ---------------------------------------------------------

typedef struct {
    Token *items;
    size_t capacity;
    size_t length;
} TokenArr;

void tokenarr_init(TokenArr *tokenarr, size_t capacity);
void tokenarr_append(TokenArr *tokenarr, Token item);
void tokenarr_free(TokenArr *tokenarr);
Token tokenarr_pop(TokenArr *tokenarr);
char *tokenarr_to_string(TokenArr *tokenarr);

// ---------------------------------------------------------

typedef struct {
    char *string;
    size_t length;
    size_t pos;
} InputStream;

void input_free(InputStream *input);
void input_init(InputStream *input, char *string);
char input_next(InputStream *input);
char input_peek(InputStream *input);
bool input_is_eof(InputStream *input);

Error input_read_symbol(TokenArr *tokens, InputStream *input);
Error input_read_number(TokenArr *tokens, InputStream *input);
Error input_read_bracket(TokenArr *tokens, InputStream *input);

// ---------------------------------------------------------

void string_remove_spaces(char *restrict str_trimmed,
                          char *restrict str_untrimmed);
/* Empty string needs to be initialized with calloc instead of malloc.
 * */
void string_append_char(char **string, char c);

bool is_symbol(char c);
bool is_bracket(char c);

Error input_tokenize(TokenArr *tokens, InputStream *input);

#endif // TOKENIZER_H
