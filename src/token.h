#ifndef TOKEN_H
#define TOKEN_H

	#include "error.h"
#include <stddef.h>
	typedef enum
	{
	    // Operators token
	    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_UN_PLUS, OP_UN_MIN,
	    // Keywords token
	    KEYW_INT, KEYW_VOID, KEYW_RETURN,
	    // Literals token
	    LIT_INT, LIT_STR,
	    // Misc token
	    COMMA, SEMI, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, CLOSE_BRACE, IDENTIFIER, EOF_TOKEN,
	} TokenType;

	typedef struct
	{
		TokenType type;
		int 	  line;
		union
		{
			long int int_val;
			char    *str_val; //holds string literal and indentifier
		} data;

	} Token;


	typedef struct
	{
		size_t   length; //How many tokens are in here.
		size_t capacity; //How many can it hold.
		Token  tklist[]; //Da token list.
	} TokenList;

	extern TokenList* list_init(size_t);
	extern void list_destroy(TokenList*);
	extern Status list_expand(TokenList**, size_t);
	extern Status list_append(TokenList**, Token*);
#endif
