#ifndef TOKENIZER_H
#define TOKENIZER_H

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

// ---------------------------------------------------------

typedef struct {
    char *value;
    TokenType type;
} Token;

void token_free(Token *token);

// ---------------------------------------------------------

typedef struct {
    Token *items;
    size_t length;
    size_t item_count;
} TokenArr;

void tokenarr_init(TokenArr *tokenarr, size_t length);
void tokenarr_append(TokenArr *tokenarr, Token item);
void tokenarr_free(TokenArr *tokenarr);
Token tokenarr_pop(TokenArr *tokenarr);

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

Token input_read_symbol(InputStream *input);
Token input_read_number(InputStream *input);
Token input_read_bracket(InputStream *input);

// ---------------------------------------------------------

void string_remove_spaces(char* restrict str_trimmed, char* restrict str_untrimmed);
void string_append_char(char *string, char c);

bool is_symbol(char c);
bool is_bracket(char c);

bool input_tokenize(TokenArr *tokens, InputStream *input);

#endif // TOKENIZER_H
