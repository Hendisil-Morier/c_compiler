#ifndef TOKEN_DATATYPE_H
#define TOKEN_DATATYPE_H

	#include "error.h"
	#include <stddef.h>
	typedef enum
	{
		ERR_TOKEN = 0,
	    // kinda operators token
	    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, INCREMENT, DECREMENT, ASSIGN_TOKEN, MODULO,
		PLUS_TOKEN, MINUS_TOKEN, ASTERISK_TOKEN, SLASH_TOKEN, EQUAL_TOKEN, COLON, QUESTION, DOT,
		MOD_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, LEFT_ASSIGN, RIGHT_ASSIGN,
		AMP_TOKEN, CARET_TOKEN, PIPE_TOKEN, TILDE_TOKEN, EXCLAIM_TOKEN, LT_TOKEN, GT_TOKEN,
		NEQUAL_TOKEN, LE_TOKEN, GE_TOKEN, LOGICAL_AND, LOGICAL_OR, LEFT_SHIFT, RIGHT_SHIFT, ARROW,
		//preprocessor directives
		PRE_INCLUDE, PRE_DEFINE, PRE_IF, PRE_IFNDEF, PRE_ELSE, PRE_ELIF, PRE_ENDIF, PRE_IFDEF, PRE_UNDEF,
		PRE_PRAGMA, PRE_ERROR, PRE_LINE,
	    // Keywords token
	    KEYW_INT, KEYW_VOID, KEYW_RETURN, KEYW_TYPEDEF, KEYW_IF, KEYW_ELSE, KEYW_CONST, KEYW_DEFAULT, KEYW_DOUBLE,
		KEYW_EXTERN, KEYW_CASE, KEYW_CONTINUE, KEYW_FLOAT, KEYW_SHORT, KEYW_REGISTER, KEYW_GOTO, KEYW_AUTO, KEYW_SIGNED, SIZEOF_OP,
		KEYW_UNION, KEYW_VOLATILE, KEYW_BREAK, KEYW_SWITCH, KEYW_UNSIGNED, KEYW_LONG, KEYW_STATIC,
		KEYW_FOR, KEYW_WHILE, KEYW_CHAR, KEYW_STRUCT, KEYW_ENUM, KEYW_DO,
	    // Literals token
	    LIT_INT, LIT_STR, LIT_CHAR,
	    // Misc token
	    COMMA, SEMI, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACK, CLOSE_BRACK, OPEN_BRACE, CLOSE_BRACE, IDENTIFIER, EOF_TOKEN,
	} TokenType;

	typedef struct
	{
		TokenType type;
		int 	  line;
		union
		{
			long int int_val;
			char*	 str_val; //holds string literal
			char char_val; //hold char literal
			const char* 	 iden_name; //Name of indentifier
		} data;

	} Token;

	typedef int char_f;
	typedef struct
	{
			const char_f* cur; //current character
			int line;
	} Lexer;
	typedef struct
	{
		size_t   length; //How many tokens are in here.
		size_t capacity; //How many can it hold.
		Token  tklist[]; //Da token list.
	} TokenList;

	typedef struct
	{
		const char* c;
		TokenType type;
	} LookupList;

#endif
