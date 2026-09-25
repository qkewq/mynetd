#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

lex_token_t *add_token(lex_token_t *current, lex_token_type_t type, size_t index, size_t length){
	lex_token_t *new = calloc(1, sizeof(lex_token_t));
	if(!new){
		return NULL;
	}

	new->type = type;
	new->index = index;
	new->length = length;
	current->next = new;

	return new;
}

lex_token_t *add_token_eof(lex_token_t *current){
	return add_token(current, LEX_EOF, 0, 0);
}

lex_token_t *add_token_lf(lex_token_t *current, size_t index){
	return add_token(current, LEX_LF, index, 1);
}

lex_token_t *add_token_assign(lex_token_t *current, size_t index){
	return add_token(current, LEX_ASSIGNMENT, index, 1);
}

int is_whitespace(char c){
	switch(c){
		case ' ':
		case '\t':
			return 1;
	}

	return 0;
}

int is_comment(char c){
	switch(c){
		case ';':
		case '#':
			return 1;
	}

	return 0;
}

int is_lf(char c){
	if(c == '\n'){
		return 1;
	}

	return 0;
}

int scanto_ex(char *conf, long conf_size, int current, char scanto){ // exclusive, the index right before the desired
	while(current < conf_size){
		if(conf[current] == scanto){
			return current - 1;
		}
		current++;
	}

	return current - 1;
}

int get_strlen(char *conf, long conf_size, int current){
	int len = 0;
	while(current < conf_size){
		if(is_whitespace(conf[current]) || is_lf(conf[current]) || conf[current] == '='){
			return len;
		}
		len++;
	}

	return len;
}

lex_token_t *lex(char *conf, long conf_size){
	lex_token_t *head = calloc(1, sizeof(lex_token_t));
	if(!head){
		return NULL;
	}
	lex_token_t *current = head;
	current->type = LEX_START;

	for(int i = 0; i < conf_size; i++){
		if(is_whitespace(conf[i])){
			continue;
		}

		if(is_lf(conf[i])){
			current = add_token_lf(current, i);
			if(!current){
				return free_tokens(head);
			}
			continue;
		}

		if(conf[i] == '='){
			current = add_token_assign(current, i);
			if(!current){
				return free_tokens(head);
			}
			continue;
		}

		if(current->type == LEX_LF || current->type == LEX_START){
			if(is_comment(conf[i])){
				i = scanto_ex(conf, conf_size, i, '\n');
				continue;
			}
		}

		int str_len = get_strlen(conf, conf_size, current);
		current = add_token(current, LEX_STRING, i, str_len);
		if(!current){
			return free_tokens(head);
		}
		i += str_len;
	}

	current = add_token_eof(current);
	if(!current){
		return free_tokens(head);
	}

	return head;
}

void *free_tokens(lex_token_t *tokens){ // always returns null
	lex_token_t *next;

	while(tokens){
		next = tokens->next;
		free(tokens);
		tokens = next;
	}

	return NULL;
}
