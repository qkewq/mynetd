#include <stdlib.h>
#include <string.h>

#include "config/syntax.h"
#include "config/lexer.h"
#include "data/bitmask.h"

bitmask_t *set_keys(){
	bitmask_t *keys = bitmask_init(LEX_TOTAL_TYPES);
	if(!keys){
		return NULL;
	}

	set_bit(keys, LEX_LOG_LEVEL);
	set_bit(keys, LEX_RATE_LIMITING);
	set_bit(keys, LEX_TCP);
	set_bit(keys, LEX_UDP);
	set_bit(keys, LEX_ADDRESS);
	set_bit(keys, LEX_PORT);
	set_bit(keys, LEX_FORMAT);
	set_bit(keys, LEX_FILEPATH);

	return keys;
}

bitmask_t *set_values(){
	bitmask_t *values = bitmask_init(LEX_TOTAL_TYPES);
	if(!values){
		return NULL;
	}

	set_bit(values, LEX_DEBUG);
	set_bit(values, LEX_INFO);
	set_bit(values, LEX_WARN);
	set_bit(values, LEX_ERROR);
	set_bit(values, LEX_FATAL);
	set_bit(values, LEX_NONE);
	set_bit(values, LEX_LIGHT);
	set_bit(values, LEX_MEDIUM);
	set_bit(values, LEX_STRICT);
	set_bit(values, LEX_ENABLED);
	set_bit(values, LEX_DISABLED);
	set_bit(values, LEX_ISO8601);
	set_bit(values, LEX_STRING);

	return values;
}

bitmask_t *set_blocks(){
	bitmask_t *blocks = bitmask_init(LEX_TOTAL_TYPES);
	if(!blocks){
		return NULL;
	}

	set_bit(blocks, LEX_MYNETD);
	set_bit(blocks, LEX_ECHO);
	set_bit(blocks, LEX_QOTD);
	set_bit(blocks, LEX_TIME);
	set_bit(blocks, LEX_DAYTIME);
	set_bit(blocks, LEX_CHARGEN);
	set_bit(blocks, LEX_DISCARD);

	return blocks;
}

int syntax_check(lex_token_t *token, char *conf){
	/*
	start
	otag -> tagid -> endtag
	string -> assignment -> string
	...
	ctag -> tagid -> endtag
	eof

	considerations:
		eof in block
		anything other than string = string
		closing id != opening id
		out of block configs
		duplicate configs in block, just take last
		dupe block allowed, just different port
		open tag in block
		close tag outside of block
	*/

	bitmask_t *keys = set_keys();
	bitmask_t *values = set_values();
	bitmask_t *blocks = set_blocks();
	bitmask_t *expects = bitmask_init(LEX_TOTAL_TYPES);
	if(!keys || !values || !blocks || !expects){
		bitmask_free(keys);
		bitmask_free(values);
		bitmask_free(blocks);
		bitmask_free(expects);

		return 0;
	}
	set_bit(expects, LEX_START);

	lex_token_type_t current_block = LEX_NULL_TOKEN;
	lex_token_t *current = token;
	int reached_eof = 0;
	int error = 0;

	while(current && !error){
		if(!is_set(expects, current->type)){
			error = 1;
			break;
		}

		switch(current->type){
			case LEX_START:
				set_bit(expects, LEX_EOF);
				set_bit(expects, LEX_LF);
				set_bit(expects, LEX_OTAG);
				break;
			case LEX_LF:
				bitmask_clear(expects);
				if(current_block == LEX_NULL_TOKEN){
					set_bit(expects, LEX_EOF);
					set_bit(expects, LEX_LF);
					set_bit(expects, LEX_OTAG);
				}
				else{
					bitmask_copy(keys, expects);
					set_bit(expects, LEX_LF);
					set_bit(expects, LEX_CTAG);
				}
				break;
			case LEX_ASSIGNMENT:
				bitmask_copy(values, expects);
				break;
			case LEX_OTAG:
			case LEX_CTAG:
				bitmask_copy(blocks, expects);
				break;
			case LEX_ENDTAG:
				bitmask_clear(expects);
				set_bit(expects, LEX_LF);
				if(current_block == LEX_NULL_TOKEN){
					set_bit(expects, LEX_EOF);
				}
				break;
			case LEX_EOF:
				reached_eof = 1;
				break;
			default:
				if(is_set(blocks, current->type)){
					if(current_block == LEX_NULL_TOKEN){
						current_block = current->type;
					}
					else if(current_block == current->type){
						current_block = LEX_NULL_TOKEN;
					}
					else{
						error = 1;
						break;
					}
					bitmask_clear(expects);
					set_bit(expects, LEX_ENDTAG);
				}
				else if(is_set(keys, current->type)){
					bitmask_clear(expects);
					set_bit(expects, LEX_ASSIGNMENT);
				}
				else if(is_set(values, current->type)){
					bitmask_clear(expects);
					set_bit(expects, LEX_LF);
				}
				else{
					error = 1;
					break;
				}
				break;
		}

		current = current->next;
	}

	bitmask_free(keys);
	bitmask_free(values);
	bitmask_free(blocks);
	bitmask_free(expects);

	if(!reached_eof || error){
		return 0;
	}

	return 1;
}
