#ifndef LEXER_H
#define LEXER_H

typedef enum lex_token_type_t{
	LEX_START = 1,
	LEX_EOF = 1 << 1,
	LEX_STRING = 1 << 2,
	LEX_ASSIGNMENT = 1 << 3,
	LEX_LF = 1 << 4,
	LEX_TAGID = 1 << 5,
	LEX_OTAG = 1 << 6,
	LEX_CTAG = 1 << 7,
	LEX_ENDTAG = 1 << 8,
	LEX_KEY = 1 << 9,
	LEX_VALUE = 1 << 10,
	LEX_LOG_LEVEL = 1 << 11,
	LEX_DEBUG = 1 << 12,
	LEX_INFO = 1 << 13,
	LEX_WARN = 1 << 14,
	LEX_ERROR = 1 << 15,
	LEX_FATAL = 1 << 16,
	LEX_RATE_LIMITING = 1 << 17,
	LEX_NONE = 1 << 18,
	LEX_LIGHT = 1 << 19,
	LEX_MEDIUM = 1 << 20,
	LEX_STRICT = 1 << 21,
	LEX_TCP = 1 << 22,
	LEX_UDP = 1 << 23,
	LEX_ENABLED = 1 << 24,
	LEX_DISABLED = 1 << 25,
	LEX_ADDRESS = 1 << 26,
	LEX_PORT = 1 << 27,
	LEX_FORMAT = 1 << 28,
	LEX_ISO8601 = 1 << 29,
	LEX_FILEPATH = 1 << 30,
	LEX_MYNETD = 1 << 31,
	LEX_ECHO = 1 << 32,
	LEX_QOTD = 1 << 33,
	LEX_TIME = 1 << 34,
	LEX_DAYTIME = 1 << 35,
	LEX_CHARGEN = 1 << 36,
	LEX_DISCARD = 1 << 37,
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
