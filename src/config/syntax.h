#ifndef SYNTAX_H
#define SYNTAX_H

typedef struct lex_token_t lex_token_t;

int syntax_check(lex_token_t *token, char *conf);

#endif
