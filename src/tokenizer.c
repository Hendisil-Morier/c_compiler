#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "token_datatype.h"
#include "error.h"


static size_t safe_mul(size_t a, size_t b)
{
	if(a == 0 || b == 0) return 0;
	if(a > (SIZE_MAX/b)) return 0; //overflow
	return a*b;
}

static size_t safe_size_calc(size_t capacity)
{
	size_t mul = safe_mul(capacity, sizeof(Token));

	//multiplication overflow.
	if(mul==0 && capacity!=0)
		return 0;
	//addition overflow. your os would prolly collapse first
	//before trying to allocate near SIZE_MAX amount of bytes. But who knows.
	//Not me, that's for sure.
	if(mul > SIZE_MAX - sizeof(TokenList))
		return 0;

	return sizeof(TokenList) + mul;
}

//construct helper.
TokenList* list_init(size_t capacity)
{

	size_t total_size = safe_size_calc(capacity);
	if(total_size == 0) return NULL;

	TokenList *list = malloc(total_size);
	if (list!=NULL)
	{
		list->length = 0;
		list->capacity = capacity;
	}

	return list;
}

void list_destroy(TokenList* list)
{
	if(list != NULL)
	{
		for(size_t i = 0; i < list->length; i++)
		{
			TokenType t = list->tklist[i].type;
			if(t == LIT_STR || t == IDENTIFIER)
				free(list->tklist[i].data.str_val);
		}
		free(list);
	}
	else
		return;
}

Status list_expand(TokenList** list_ptr, size_t new_cap)
{
	if (list_ptr == NULL) return FAILURE;

	TokenList* list = *list_ptr;

	if (list == NULL) return FAILURE;
	if (new_cap <= list->capacity) return NOT_EXECUTE;

	size_t new_size = safe_size_calc(new_cap);
	if(new_size == 0) return FAILURE;

	TokenList *new_list = realloc(list, new_size);

	if(new_list == NULL) return FAILURE;
	new_list->capacity = new_cap;
	*list_ptr = new_list;
	return SUCCESS;
}

Status list_append(TokenList** list_ptr, Token* token)
{
	if(list_ptr == NULL || token == NULL)
		return FAILURE;

	TokenList* list = *list_ptr;
	if(list==NULL) return FAILURE;

	if(list->length == list->capacity)
	{
		size_t new_size = (list->capacity==0)? 10 : list->capacity * 1.5;
		Status stat = list_expand(list_ptr, new_size);
		if(status_isequal(stat, FAILURE))
			return FAILURE;
	}

	list->tklist[list->length++] = *token;
	return SUCCESS;
}
