#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>

#include "parser.h"
#include "lexer.h"
#include "syntax.h"

typedef enum parser_return_t{
	PARSE_SUCCESS = 0,
	PARSE_FOPEN = -1,
	PARSE_FSEEK = -2,
	PARSE_FTELL = -3,
	PARSE_MEMRY = -4,
	PARSE_FREAD = -5,
	PARSE_LEX = -6,
	PARSE_SYNTAX = -7,
} parser_return_t;

int parse_config_file(char *path, configs_t **ret){
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
		free(conf);
		return PARSE_LEX;
	}

	#ifdef LEX_DEBUG
	#include <unistd.h>
	printf("LEXER OUTPUT\n");
	lex_token_t *current = tokens;
	while(current){
		printf("TOKEN\n\ttype: ");
		switch(current->type){
			case LEX_START:
				printf("LEX_START");
				break;
			case LEX_EOF:
				printf("LEX_EOF");
				break;
			case LEX_STRING:
				printf("LEX_STRING");
				break;
			case LEX_ASSIGNMENT:
				printf("LEX_ASSIGNMENT");
				break;
			case LEX_LF:
				printf("LEX_LF");
				break;
			case LEX_TAGID:
				printf("LEX_TAGID");
				break;
			case LEX_OTAG:
				printf("LEX_OTAG");
				break;
			case LEX_CTAG:
				printf("LEX_CTAG");
				break;
			case LEX_ENDTAG:
				printf("LEX_ENDTAG");
				break;
		}
		printf("\n");
		printf("\tIndex: %d\n\tLength: %d\n", current->index, current->length);

		printf("\tString: ");
		fflush(stdout);
		if(current->type == LEX_LF){
			printf("'\\n'");
		}
		else if(current->length){
			write(STDOUT_FILENO, &conf[current->index], current->length);
		}
		printf("\n");

		current = current->next;
	}
	return 0;
	#endif

	if(!syntax_check(tokens, conf)){
		free(conf);
		free_tokens(tokens);
		return PARSE_SYNTAX;
	}

	configs_t configs = calloc(1, sizeof(configs_t));
	if(!configs){
		free(conf);
		free_tokens(tokens);
		return PARSE_MEMRY;
	}

	port_map_t configs->services = calloc(1, sizeof(port_map_t));
	if(!configs->services){
		free(conf);
		free_tokens(tokens);
		free_configs(configs);
		return PARSE_MEMRY;
	}

	configs->services = services;
}

void free_configs(configs_t *configs){

}

/*

tcp protos throttle based on bytes out per stream per second|minute?
udp protos limit based on dgrams in per source ip per second|minute?

<mynetd>
	address=127.0.0.1
	address=[::1]
	address=192.168.1.1
	rate_limit_level=[light|medium|strict]
	log_level=[the normal ones]
</mynetd>

<echo>
	port=7
	address=127.0.0.1
</echo>

*/
