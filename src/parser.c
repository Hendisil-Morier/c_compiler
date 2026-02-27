#include "ast.h"
#include "token_datatype.h"
#include "error.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct
{
	TokenList * list;
	int position;
} Parser;

extern const char* token_type_name(TokenType t);

//peek at the next token without consuming.
static Token* peek(Parser* parser)
{
	if (parser == NULL) return NULL;
	if (parser->position + 1 >= parser->list->length) return NULL;

	return &parser->list->tklist[parser->position + 1];
}

//look at the current token without consuming
static Token* look(Parser* parser)
{

	if (parser == NULL) return NULL;
	if (parser->position >= parser->list->length) return NULL;

	return &parser->list->tklist[parser->position];
}

bool is_expr(TokenType type)
{
	switch(type)
	{
	case LIT_INT:
		return true;
	default:
		return false;
	}
}

static Status advance(Parser* parser)
{
	if (parser == NULL) return FAILURE;
	if (parser->position >= parser->list->length) return FAILURE;

	parser->position++;
	return SUCCESS;
}

static Status expect(TokenType expected, TokenType actual)
{
	if (actual != expected)
	{
		printf("Expected %s, found %s.\n", token_type_name(expected), token_type_name(actual));
		return FAILURE;
	}

	return SUCCESS;
}

//parse expresion (recursive soon, int const for now) and ADVANCE
astNode* parse_expr(Parser* parser)
{
	if (parser == NULL) return NULL;
	Token* cur = look(parser);
	if (cur==NULL) return NULL;
	Status st = FAILURE;

	st = expect(LIT_INT, cur->type);

	if (status_isequal(st, FAILURE)) return NULL;
	int value = cur->data.int_val;

	astNode* expr_node = malloc(sizeof(astNode));
	if (expr_node == NULL) return NULL;

	advance(parser);
	expr_node->type = AST_INT_CONST;
	expr_node->nodeData.int_constant.value = value;

	return expr_node;
}

//parse return statement and ADVANCE
astNode* parse_return(Parser* parser)
{
	if (parser == NULL) return NULL;
	TokenType type = parser->list->tklist[parser->position].type;
	Status st = FAILURE;

	st = expect(KEYW_RETURN, type);
	if (status_isequal(st, FAILURE)) return NULL;

	st = advance(parser);
	if(status_isequal(st,FAILURE)) return NULL;

	Token* cur = look(parser);
	if (cur == NULL) return NULL;

	if (is_expr(cur->type) == false)
	{
		st = expect(SEMI, cur->type);
		if (status_isequal(st, FAILURE)) return NULL;
		advance(parser);
		astNode* return_node = malloc(sizeof(astNode));
		if (return_node == NULL) return NULL;

		return_node->type = AST_RETURN_STMT;
		return_node->nodeData.return_stmt.expr = NULL;

		return return_node;
	}
	else
	{

		astNode* expr_node = parse_expr(parser);
		if (expr_node == NULL) return NULL;

		Token* next = look(parser);
		if (next == NULL) return NULL;
		st = expect(SEMI, next->type);
		if (status_isequal(st, FAILURE)) return NULL;
		advance(parser);

		astNode* return_node = malloc(sizeof(astNode));
		if (return_node == NULL) return NULL;

		return_node->type = AST_RETURN_STMT;
		return_node->nodeData.return_stmt.expr = expr_node;
		return return_node;
	}

	return NULL;
}
