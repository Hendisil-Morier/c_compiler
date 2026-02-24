#include "token_datatype.h"
#include "error.h"
#include <stdio.h>


extern const char* token_type_name(TokenType t);

inline Status expect(TokenType expected, TokenType actual)
{
	if (actual != expected)
	{
		printf("Expected %s, found %s.", token_type_name(expected), token_type_name(actual));
		return FAILURE;
	}

	return SUCCESS;
}

Status parse_exp(void);

Status parse(TokenList* list)
{
	Status st = FAILURE;
	for(size_t i = 0; i < list->length; i++)
	{
		Token* current_token = &list->tklist[i];

		if(current_token->type == KEYW_RETURN)
		{

		}
	}

	return st;
}
