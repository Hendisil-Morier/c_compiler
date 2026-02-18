#ifndef STRING_DATATYPE_H
#define STRING_DATATYPE_H

#include <stddef.h>

typedef struct
{
	char** string;
	size_t length;
	size_t capacity;
} stringIntern;

#endif
