#include <stdio.h>
#include "error.h"

FILE* inFile;

typedef struct
{
	const char* cur; //current character
	int line;
} Lexer;

void skip_whitespace(Lexer* lex)
{
	char c = *(lex->cur);

	while (c == ' ' || c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v')
	{
		if(c=='\n') lex->line++;
		lex->cur++;
		c = *(lex->cur);
	}
}

Status read_file(const char* path)
{
	FILE* file = fopen(path, "rb");
	if(file == NULL) return FAILURE;

	inFile = file;
	return SUCCESS;
}
