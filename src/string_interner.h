#ifndef STRING_INTERNER_H
#define STRING_INTERNER_H

	#include "string_datatype.h"

	extern stringIntern* ident_intern;

	stringIntern* new_interner();
	stringIntern* intern_append(stringIntern*, const char*);
	const char* stringIntern_find(const stringIntern*, const char*);
	void destroy_iden_intern();

#endif
