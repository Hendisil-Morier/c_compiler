#ifndef TOKEN_DATATYPE_H
#define TOKEN_DATATYPE_H

	#include "error.h"
	#include <stddef.h>
	typedef enum
	{
		ERR_TOKEN = 0,
	    // kinda operators token
	    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, INCREMENT, DECREMENT, ASSIGN_TOKEN,
		PLUS_TOKEN, MINUS_TOKEN, ASTERISK_TOKEN, SLASH_TOKEN, EQUAL_TOKEN,
		//preprocessor directives
		PRE_INCLUDE, PRE_DEFINE, PRE_IF, PRE_IFNDEF, PRE_ELSE, PRE_ELIF, PRE_ENDIF,
	    // Keywords token
	    KEYW_INT, KEYW_VOID, KEYW_RETURN, KEYW_TYPEDEF, KEYW_IF, KEYW_ELSE, KEYW_CONST, KEYW_DEFAULT, KEYW_DOUBLE,
		KEYW_EXTERN, KEYW_CASE, KEYW_CONTINUE, KEYW_FLOAT, KEYW_SHORT, KEYW_REGISTER, KEYW_GOTO, KEYW_AUTO, KEYW_SIGNED, SIZEOF_OP,
		KEYW_UNION, KEYW_VOLATILE, KEYW_BREAK, KEYW_SWITCH, KEYW_UNSIGNED, KEYW_LONG, KEYW_STATIC,
		KEYW_FOR, KEYW_WHILE, KEYW_CHAR, KEYW_STRUCT, KEYW_ENUM, KEYW_DO,
	    // Literals token
	    LIT_INT, LIT_STR,
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
			char* 	 iden_name; //Name of indentifier
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
	} KeywordList;

	const KeywordList keyword_list[] =
	{
  		{"auto",        KEYW_AUTO},
	    {"break",       KEYW_BREAK},
	    {"case",        KEYW_CASE},
	    {"char",        KEYW_CHAR},
	    {"const",       KEYW_CONST},
	    {"continue",    KEYW_CONTINUE},
	    {"default",     KEYW_DEFAULT},
	    {"do",          KEYW_DO},
	    {"double",      KEYW_DOUBLE},
	    {"else",        KEYW_ELSE},
	    {"enum",        KEYW_ENUM},
	    {"extern",      KEYW_EXTERN},
	    {"float",       KEYW_FLOAT},
	    {"for",         KEYW_FOR},
	    {"goto",        KEYW_GOTO},
	    {"if",          KEYW_IF},
	    {"int",         KEYW_INT},
	    {"long",        KEYW_LONG},
	    {"register",    KEYW_REGISTER},
	    {"return",      KEYW_RETURN},
	    {"short",       KEYW_SHORT},
	    {"signed",      KEYW_SIGNED},
	    {"sizeof",      SIZEOF_OP},
	    {"static",      KEYW_STATIC},
	    {"struct",      KEYW_STRUCT},
	    {"switch",      KEYW_SWITCH},
	    {"typedef",     KEYW_TYPEDEF},
	    {"union",       KEYW_UNION},
	    {"unsigned",    KEYW_UNSIGNED},
	    {"void",        KEYW_VOID},
	    {"volatile",    KEYW_VOLATILE},
	    {"while",       KEYW_WHILE},
	};

#endif
