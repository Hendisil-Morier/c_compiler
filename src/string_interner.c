#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "string_datatype.h"

const char *Keywords[] =  {"int", "void", "return"};
const size_t keywords_size = sizeof(Keywords) / sizeof(char*);
const stringIntern keyword_intern = {(char**)Keywords, keywords_size, keywords_size}; //do not free this one
stringIntern* ident_intern = NULL; //free this one instead


stringIntern* new_interner()
{
	stringIntern* new_intern = malloc(sizeof(stringIntern));
	if (new_intern == NULL) return NULL;

	new_intern->string = NULL;
	new_intern->capacity = 0;
	new_intern->length = 0;

	return new_intern;
}

stringIntern* intern_append(stringIntern* intern, const char* str)
{
	if (intern == NULL || str == NULL) return NULL;

	size_t length, capacity;
	char** current_array = intern->string;
	length   = intern->length;
	capacity = intern->capacity;


	if(length == capacity)
	{
		size_t new_size = capacity*3/2 +1;
		char** new_arr = realloc(current_array, new_size*sizeof(char*));

		if(new_arr==NULL) return NULL;

		current_array = intern->string = new_arr;
		intern->capacity = new_size;
	}

	size_t str_size = strlen(str) + 1;
	char* p = strdup(str);
	if(p == NULL) return NULL;

	intern->string[length] = p;

	intern->length++;

	return intern;
}

const char* stringIntern_find(const stringIntern* intern, const char* str)
{
	if (intern == NULL || str == NULL) return NULL;

	for( size_t i = 0; i < intern->length; i++)
	{
		if(strcmp(intern->string[i], str) == 0)
			return intern->string[i];
	}
	return NULL;
}

void destroy_iden_intern()
{
	if (ident_intern == NULL) return;
	if(ident_intern->string == NULL) return;
	for(size_t i = 0; i < ident_intern->length; i++)
		free(ident_intern->string[i]);

	free(ident_intern->string);
	free(ident_intern);
}
