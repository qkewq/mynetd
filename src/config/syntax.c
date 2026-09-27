#include <stdlib.h>
#include <string.h>


#include "syntax.h"
#include "lexer.h"

int tag_cmp(lex_token_t *otag, lex_token_t *ctag, char *conf){
	if(otag->length != ctag->length){
		return 0;
	}

	if(strncmp(conf + otag->index, conf + ctag->index, otag->length) != 0){
		return 0;
	}

	return 1;
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

	int wait_key = 0;
	int key_present = 0;
	// int line_num = 1; // Will add logging later :)
	// int last_lf_index = 0; // :)
	lex_token_t *current = token->next; // Always at least two tokens, start & eof
	lex_token_t *current_tagid = NULL;
	lex_token_type_t expects = (LEX_LF | LEX_OTAG);
	lex_token_type_t next_lf = 0;

	while(current->type != LEX_EOF){ //segfault??
		if(!(current->type & expects)){
			return 0;
		}

		switch(current->type){ // ts is c soup :( state machine the wrong way round
			case LEX_STRING:
				if(wait_key){
					wait_key = 0;
					key_present = 1;
					expects = LEX_ASSIGNMENT;
				}
				if(key_present){
					key_present = 0;
					wait_key = 1;
					expects = LEX_LF;
					next_lf = (LEX_CTAG | LEX_STRING);
				}
				break;
			case LEX_ASSIGNMENT:
				expects = LEX_STRING;
				break;
			case LEX_LF:
				expects |= (next_lf | LEX_LF);
				next_lf = 0;
				break;
			case LEX_TAGID:
				current_tagid = current;
				expects = LEX_ENDTAG;
				if(!current_tagid){
					current_tagid = current;
				}
				else{
					current_tagid = NULL;
				}
				break;
			case LEX_OTAG:
				expects = LEX_TAGID;
				break;
			case LEX_CTAG:
				wait_key = 0;
				if(!tag_cmp(current_tagid, current, conf)){
					return 0;
				}
				expects = LEX_LF;
				next_lf = LEX_OTAG;
				break;
			case LEX_ENDTAG:
				expects = LEX_LF;
				if(current_tagid){
					next_lf = LEX_STRING;
					wait_key = 1;
				}
				else{
					next_lf = LEX_OTAG;
				}
				break;
		}
		current = current->next;
	}

	if(current_tagid){
		return 0;
	}

	return 1;
}
