#ifndef LEXER_H
#define LEXER_H

typedef enum lex_token_type_t{
	LEX_START = 0x01,
	LEX_EOF = 0x02,
	LEX_STRING = 0x04,
	LEX_ASSIGNMENT = 0x08,	// '='
	LEX_LF = 0x10,			// '\n'
	LEX_TAGID = 0x20,
	LEX_OTAG = 0x40,		// '<'
	LEX_CTAG = 0x80,		// '</'
	LEX_ENDTAG = 0x100,		// '>'
	LEX_KEY = 0x200,
	LEX_VALUE = 0x0400,
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
