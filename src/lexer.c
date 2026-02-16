#include <stdio.h>
#include "error.h"
#include "token_datatype.h"
#include "tokenizer.h"

// insanely unsafe.
// for convinient only. this thing have no type check.
#define iden_condition(x) (x >= 'A' && x <= 'Z' || x >= 'a' && x <= 'z' || x == '_')

FILE* inFile;
typedef int char_f; //using int to hold char look illogical
typedef struct
{
	const char_f* cur; //current character
	int line;
} Lexer;

void skip_whitespace(Lexer* lex)
{
	char_f c = *(lex->cur);

	while (c == ' ' || c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v')
	{
		if(c=='\n') lex->line++;
		c = *(lex->cur++);
	}
}

Status read_file(const char* path)
{
	FILE* file = fopen(path, "rb");
	if(file == NULL) return FAILURE;

	inFile = file;
	return SUCCESS;
}

TokenList* get_token(Lexer* lex)
{
	lex->line = 1;

	TokenList* new_list = list_init(10);
	if (new_list == NULL) return NULL;
	Status st = FAILURE;
	char_f cur_char;

	while((cur_char = *lex->cur) != EOF)
	{
		skip_whitespace(lex);
		Token new_token = {0};
		switch (cur_char)
		{
		case '(' :
			lex->cur++;
			new_token.type = OPEN_PAREN;
			break;
		case ')' :
			lex->cur++;
			new_token.type = CLOSE_PAREN;
			break;
		case '{' :
			lex->cur++;
			new_token.type = OPEN_BRACE;
			break;
		case '}' :
			lex->cur++;
			new_token.type = CLOSE_BRACE;
			break;
		case ',' :
			lex->cur++;
			new_token.type = COMMA;
			break;
		case ';' :
			lex->cur++;
			new_token.type = SEMI;
			break;
		}

		if (cur_char >= '0' && cur_char <= '9')
		{
			char_f c = *lex->cur;
			int value = 0;
			while(c >= '0' && c <= '9')
			{
				value = value*10 + (c - '0');
				c = *(lex->cur++);
			}
			new_token.type = LIT_INT;
			new_token.data.int_val  = value;
		}

		if (iden_condition(cur_char))
		{
			char_f c = *lex->cur++;
			while (c >= '0' && c <= '9' || iden_condition(c))
			{

			}
		}

		new_token.line = lex->line;
		st = list_append(&new_list, &new_token);
		if (status_isequal(st, FAILURE)) return NULL;
	}

	if(cur_char == EOF)
	{
		Token eof_tok = {.type = EOF_TOKEN};
		list_append(&new_list, &eof_tok);
	}


}
