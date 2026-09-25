#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "parser.h"
#include "lexer.h"

typedef enum parser_return_t{
	PARSE_SUCCESS = 0,
	PARSE_FOPEN = -1,
	PARSE_FSEEK = -2,
	PARSE_FTELL = -3,
	PARSE_MEMRY = -4,
	PARSE_FREAD = -5,
	PARSE_LEX = -6,

} parser_return_t;

int parse_config_file(char *path, configs_t *ret){
	FILE *file = fopen(path, "rb");
	if(!file){
		return PARSE_FOPEN;
	}

	if(fseek(file, 0, SEEK_END) == -1){
		fclose(file);
		return PARSE_FSEEK;
	}

	long conf_size = ftell(file);
	if(conf_size == -1){
		fclose(file);
		return PARSE_FTELL;
	}

	rewind(file);

	char *conf = calloc(conf_size, sizeof(char));
	if(!conf){
		fclose(file);
		return PARSE_MEMRY;
	}

	if(fread(conf, 1, conf_size, file) != conf_size){
		fclose(file);
		free(conf);
		return PARSE_FREAD;
	}

	fclose(file);

	lex_token_t *tokens = lex(conf, conf_size);
	if(!tokens){
		return PARSE_LEX;
	}

}

/*

<mynetd>
	addrs=blahblah
</mynetd>

<echo>
	port=7
</echo>

*/
