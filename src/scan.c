#include <stdio.h>
#include <ctype.h>
#include "error.h"
#include "token.h"

extern FILE* inFile;
extern int Putback;

int next_char()
{
	int c;
	if(Putback)
	{
		c = Putback;
		Putback = 0;
		return c;
	}
	c = fgetc(inFile);
	return c;
}

int skip_whitespace()
{
	int c = next_char();
	while(isspace(c))
		c = next_char();
	return c;
}

Status read_file(const char* path)
{
	FILE* file = fopen(path, "rb");
	if(file == NULL) return FAILURE;

	inFile = file;
	return SUCCESS;
}

Status scan()
{
	int c = 0;
	while(c!=EOF)
	{
		c = skip_whitespace();
	}
}
