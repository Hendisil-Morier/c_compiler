#include <stdio.h>
#include <string.h>
#include "error.h"
#include "token_datatype.h"
#include "tokenizer.h"
#include "string_interner.h"

// insanely unsafe.
// for convinient only. these thing have only one use and will fuck the code up in other places.
#define iden_condition(x) (x >= 'A' && x <= 'Z' || x >= 'a' && x <= 'z' || x == '_')
#define is_single(c) (cur_char == c)
#define IDENT_BUFFER_SIZE 256

extern FILE* inFile;

void skip_whitespace(Lexer* lex)
{
	while (1)
	{
		char_f c = *(lex->cur);
		if(c==EOF) break;
		if(c=='\n') lex->line++;
		if(c == ' ' || c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v')
		{
			(lex->cur++);
		}
		else break;
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

	TokenList* new_list = list_init(10);
	if (new_list == NULL) return NULL;
	Status st = FAILURE;
	char_f cur_char = *lex->cur;

	while(1)
	{
		if(cur_char == EOF) break;
		skip_whitespace(lex);
		cur_char = *lex->cur;
		Token new_token = {0};

		if (is_single('('))
		{
			lex->cur++; new_token.type = OPEN_PAREN;
		}
		else if (is_single(')'))
		{
			lex->cur++; new_token.type = CLOSE_PAREN;
		}
		else if (is_single('{'))
		{
			lex->cur++;	new_token.type = OPEN_BRACE;
		}
		else if (is_single('}'))
		{
			lex->cur++;	new_token.type = CLOSE_BRACE;
		}
		else if (is_single(','))
		{
			lex->cur++;	new_token.type = COMMA;
		}
		else if (is_single(';'))
		{
			lex->cur++;	new_token.type = SEMI;
		}
		else if (cur_char >= '0' && cur_char <= '9')
		{
			int value = 0;
			while(*lex->cur >= '0' && *lex->cur <= '9')
			{
				value = value*10 + (*lex->cur - '0');
				lex->cur++;
			}
			new_token.type = LIT_INT;
			new_token.data.int_val  = value;
		}
		else if (iden_condition(cur_char))
		{
    		char buffer[IDENT_BUFFER_SIZE];
     		size_t i = 0;

		    // Read characters while they are valid identifier chars
		    while (i < IDENT_BUFFER_SIZE - 1 && (iden_condition(*lex->cur) || (*lex->cur >= '0' && *lex->cur <= '9')))
			{
		        buffer[i++] = *lex->cur;
		        lex->cur++;
		    }
		    buffer[i] = '\0';

		    // Check if it's a keyword
		    if (strcmp(buffer, "int") == 0)
		        new_token.type = KEYW_INT;
		    else if (strcmp(buffer, "return") == 0)
		        new_token.type = KEYW_RETURN;
		    else if (strcmp(buffer, "void") == 0)
		        new_token.type = KEYW_VOID;
		    else
		    {
		        // It's an identifier – intern it
		        const char* interned = stringIntern_find(ident_intern, buffer);
		        if (!interned) {
		            // Not yet interned: add it
		            intern_append(ident_intern, buffer);
		            interned = ident_intern->string[ident_intern->length - 1]; // pointer to the new copy
		        }
		        new_token.type = IDENTIFIER;
		        new_token.data.iden_name = (char*)interned;   // cast away const – but the interner owns it
		    }
		}
		else
		{
			lex->cur++;
			new_token.type = ERR_TOKEN;
			//continue;
		}

		new_token.line = lex->line;
		st = list_append(&new_list, &new_token);
		if (status_isequal(st, FAILURE))
			{list_destroy(new_list); return NULL;}
	}

	Token eof_tok = {.type = EOF_TOKEN};
	list_append(&new_list, &eof_tok);
	return new_list;
}
