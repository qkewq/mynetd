#ifndef LEXER_H
#define LEXER_H

typedef enum lex_token_type_t{
	LEX_START = 0,
	LEX_EOF,
	LEX_STRING,
	LEX_ASSIGNMENT,
	LEX_LF,
	LEX_TAGID,
	LEX_OTAG,
	LEX_CTAG,
	LEX_ENDTAG,
} lex_token_type_t;

typedef struct lex_token_t{
	struct lex_token_t *next;
	enum lex_token_type_t type;
	size_t index;
	size_t length;
} lex_token_t;

lex_token_t *lex(char *conf, long conf_size);
void *free_tokens(lex_token_t *tokens);

#endif
