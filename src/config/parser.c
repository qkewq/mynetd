#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>

#include "parser.h"
#include "lexer.h"
#include "syntax.h"

#define ALL_SERVICES (SERV_MYNETD | SERV_ECHO | SERV_QOTD | SERV_TIME | SERV_DAYTIME | SERV_CHARGEN | SERV_DISCARD)

typedef struct valid_configs_t{ // scuffed
	enum services_t services;
	char *key;
	char **values;
} valid_configs_t;

valid_configs_t keys_values[] = {
	{
		.services = ALL_SERVICES,
		.key = "log_level",
		.values = {"debug", "info", "warn", "error", "fatal"}
	},
	{
		.services = ALL_SERVICES,
		.key = "rate_limiting",
		.values = {"none", "light", "medium", "strict"}
	},
	{
		.services = ALL_SERVICES,
		.key = "tcp",
		.values = {"enabled", "disabled"}
	},
	{
		.services = ALL_SERVICES,
		.key = "udp",
		.values = {"enabled", "disabled"}
	},
	{
		.services = ALL_SERVICES,
		.key = "address",
		.values = NULL
	}
	{
		.services = (ALL_SERVICES & !(SERV_MYNETD)),
		.key = "port",
		.values = NULL
	},
	{
		.services = (SERV_DAYTIME),
		.key = "format",
		.values = {"iso8601"}
	},
	{
		.services = (SERV_CHARGEN | SERV_QOTD),
		.key = "filepath",
		.values = NULL
	},
};

typedef struct service_names_t{
	char *name;
	enum services_t service;
} service_names_t;

service_names_t tag_ids[] = {
	{
		.name = "mynetd",
		.service = SERV_MYNETD
	},
	{
		.name = "echo",
		.service = SERV_ECHO
	},
	{
		.name = "qotd",
		.service = SERV_QOTD
	},
	{
		.name = "time",
		.service = SERV_TIME
	},
	{
		.name = "daytime",
		.service = SERV_DAYTIME
	},
	{
		.name = "chargen",
		.service = SERV_CHARGEN
	},
	{
		.name = "discard",
		.service = SERV_DISCARD
	},
};

services_t validate_tagid(char *name, int length){
	for(int i = 0; i < sizeof(tag_ids) / sizeof(service_names_t); i++){
		if(strlen(tag_ids[i].name) != length){
			continue;
		}
		if(strncmp(tag_ids[i].name, name, length) == 0){
			return tag_ids[i].service;
		}
	}

	return 0;
}

int set_value(char *conf, lex_token_t *key, lex_token_t* value, services_t service, service_config_t new){
	int key_index = -1;
	for(int i = 0; i < sizeof(keys_values) / sizeof(valid_configs_t); i++){
		if(strlen(keys_values->key) != key->length){
			continue;
		}
		if(strncmp(conf[key->index], keys_values->key, key->length) == 0){
			key_index = i;
			break;
		}
	}

	if(key_index == -1){
		return 0;
	}
	if(!(keys_values->services & service)){
		return 0;
	}

	for(int i = 0; i < sizeof(keys_values[key_index].values) / sizeof(char *); i++){

	}
}

typedef enum parser_return_t{
	PARSE_SUCCESS = 0,
	PARSE_FOPEN = -1,
	PARSE_FSEEK = -2,
	PARSE_FTELL = -3,
	PARSE_MEMRY = -4,
	PARSE_FREAD = -5,
	PARSE_LEX = -6,
	PARSE_SYNTAX = -7,
	PARSE_INVALID = -8,
} parser_return_t;

service_config_t *new_service(services_t type){
	service_config_t *new = calloc(1, sizeof(service_config_t));
	if(!new){
		return NULL;
	}

	new->service = type;
	return new;
}

int read_tokens(configs_t *configs, char *conf, lex_token_t *tokens){
	lex_token_t *current = tokens;
	lex_token_t current_key = NULL;
	services_t current_block = 0;
	service_config_t *new = NULL;

	while(current->type != LEX_EOF){
		switch(current->type){
			case LEX_TAGID:
				if(!current_block){
					current_block = validate_tagid(&conf[current->index], current->length);
					if(!current_block){
						return PARSE_INVALID;
					}
					new = new_service(current_block);
					if(!new){
						return PARSE_MEMRY;
					}
					new->next = configs->services;
					configs->services = new;
				}
				break;
			case LEX_KEY:
				current_key = current;
				break;
			case LEX_VALUE:
				set_value(conf, current_key, current, current_block, new);
				break;
		}

		current = current->next;
	}
}

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
		printf("TOKEN\n\ttype: %d\n", current->type);
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

	configs_t *configs = calloc(1, sizeof(configs_t));
	if(!configs){
		free(conf);
		free_tokens(tokens);
		return PARSE_MEMRY;
	}

	configs->services = calloc(1, sizeof(port_map_t));
	if(!configs->services){
		free(conf);
		free_tokens(tokens);
		free_configs(configs);
		return PARSE_MEMRY;
	}

	lex_token_t *current = tokens;
	services_t current_block = 0;
	lex_token_t *current_key = NULL;
	while(current->type != LEX_EOF){
		switch(current->type){
			case LEX_TAGID:
				if(!current_block){
					current_block = validate_tagid(&conf[current->index], current->length);
					if(!current_block){
						return PARSE_INVALID;
					}
				}
				else{
					current_block = 0;
				}
				break;
			case LEX_KEY:
				break;
			case LEX_VALUE:
				break;
			default:
				break;
		}


		current = current->next;
	}
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
