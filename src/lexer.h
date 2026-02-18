#ifndef LEXER_H
#define LEXER_H

	#include "error.h"
	#include "token_datatype.h"
	extern Status read_file(const char*);
	extern TokenList* get_token(Lexer* lex);

#endif
