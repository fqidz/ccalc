#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"

Error tokens_to_postfix(TokenArr *tokens, char *input_string);
Error evaluate_postfix_tokens(double *result, TokenArr *tokens);

#endif /* ifndef PARSE_H */
