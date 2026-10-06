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
	LEX_KEY,
	LEX_VALUE,
	LEX_LOG_LEVEL,
	LEX_DEBUG,
	LEX_INFO,
	LEX_WARN,
	LEX_ERROR,
	LEX_FATAL,
	LEX_RATE_LIMITING,
	LEX_NONE,
	LEX_LIGHT,
	LEX_MEDIUM,
	LEX_STRICT,
	LEX_TCP,
	LEX_UDP,
	LEX_ENABLED,
	LEX_DISABLED,
	LEX_ADDRESS,
	LEX_PORT,
	LEX_FORMAT,
	LEX_ISO8601,
	LEX_FILEPATH,
	LEX_MYNETD,
	LEX_ECHO,
	LEX_QOTD,
	LEX_TIME,
	LEX_DAYTIME,
	LEX_CHARGEN,
	LEX_DISCARD,
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
