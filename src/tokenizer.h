#ifndef TOKENIZER_H
#define TOKENIZER_H

	#include "token_datatype.h"
	extern TokenList* list_init(size_t);
	extern void list_destroy(TokenList*);
	extern Status list_expand(TokenList**, size_t);
	extern Status list_append(TokenList**, Token*);

#endif
