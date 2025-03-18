#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"

Error tokens_to_postfix(TokenArr *tokens, char *input_string);
Error evaluate_postfix_tokens(double *result, TokenArr *tokens);

int double_get_decimal_length(double input, int max_len);
double double_get_scientific_notation(double input, int *exponent);

char *double_format_to_string(double input);

#endif /* ifndef PARSE_H */
