#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"

void tokens_to_postfix(TokenArr *tokens);
double evaluate_postfix_tokens(TokenArr *tokens);

#endif /* ifndef PARSE_H */
