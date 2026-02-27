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

static Token* glance_back(Parser* parser)
{
	if (parser == NULL || parser->position == 0) return NULL;

	return &parser->list->tklist[parser->position - 1];
}

//look at the current token without consuming
static Token* look(Parser* parser)
{

	if (parser == NULL) return NULL;
	if (parser->position > parser->list->length) return NULL;

	return &parser->list->tklist[parser->position];
}

void free_ast(astNode* node)
{
	if (node == NULL) return;
	switch(node->type)
	{
	case AST_FUNCTION:
		free_ast(node->nodeData.function.body);
		break;
	case AST_PROGRAM:
		free_ast(node->nodeData.program.function);
		break;
	case AST_RETURN_STMT:
		free_ast(node->nodeData.return_stmt.expr);
		break;
	default:
		break;
	}

	free(node);
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

bool is_type(TokenType type)
{
	switch(type)
	{
	case KEYW_INT:
	case KEYW_CHAR:
	case KEYW_LONG:
	case KEYW_VOID:
	case KEYW_DOUBLE:
	case KEYW_FLOAT:
	case KEYW_UNSIGNED:
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

static Status expect(TokenType expected, Token* actual_t)
{
	if (actual_t == NULL)
	{
		fprintf(stderr, "Something sussy in the compiler code: [expect] function called with null pointer.");
		return FAILURE;
	}
	TokenType actual = actual_t->type;
	if (actual != expected)
	{
		printf("Expected %s, found %s at line %d.\n",
		token_type_name(expected), token_type_name(actual), actual_t->line);
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

	st = expect(LIT_INT, cur);

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
	//TokenType type = parser->list->tklist[parser->position].type;
	Token* cur = look(parser);
	Status st = FAILURE;

	st = expect(KEYW_RETURN, cur);
	if (status_isequal(st, FAILURE)) return NULL;

	st = advance(parser);
	if(status_isequal(st,FAILURE)) return NULL;


	if (cur == NULL) return NULL;

	if (is_expr(cur->type) == false)
	{
		st = expect(SEMI, cur);
		if (status_isequal(st, FAILURE)) return NULL;

		astNode* return_node = malloc(sizeof(astNode));
		if (return_node == NULL) return NULL;

		advance(parser);

		return_node->type = AST_RETURN_STMT;
		return_node->nodeData.return_stmt.expr = NULL;

		return return_node;
	}
	else
	{
		astNode* expr_node = parse_expr(parser);
		if (expr_node == NULL) return NULL;

		Token* next = look(parser);
		if (next == NULL) {free(expr_node); return NULL;}
		st = expect(SEMI, next);
		if (status_isequal(st, FAILURE)) {free(expr_node); return NULL;}


		astNode* return_node = malloc(sizeof(astNode));
		if (return_node == NULL) {free(expr_node); return NULL;}

		advance(parser);

		return_node->type = AST_RETURN_STMT;
		return_node->nodeData.return_stmt.expr = expr_node;
		return return_node;
	}

	return NULL;
}

//exactly on the tin. expect & advance
static Status expect_advance(Parser* parser, TokenType expected)
{
	if (parser == NULL) return FAILURE;

	Token* cur = look(parser);
	if (cur == NULL) return FAILURE;

	Status st = expect(expected, cur);
	if (status_isequal(st, FAILURE)) return FAILURE;

	st = advance(parser);
	if (status_isequal(st, FAILURE)) return FAILURE;

	return SUCCESS;
}

astNode* parse_function(Parser* parser)
{
	//initial pointer validation
	if (parser == NULL) return NULL;
	Token* cur = look(parser);
	if(cur==NULL) return NULL;
	const char* func_name = NULL;
	astNode* func_node = NULL;
	astNode* return_node = NULL;

	if (is_type(cur->type) == false) return NULL; //check if the current token is a type token
	Status st = FAILURE;

	st = expect_advance(parser, IDENTIFIER);
	if(status_isequal(st, FAILURE)) return NULL;
	Token* back = glance_back(parser);
	func_name = back->data.iden_name;

	st = expect_advance(parser, OPEN_PAREN);
	if(status_isequal(st, FAILURE)) return NULL;

	st = expect_advance(parser, CLOSE_PAREN);
	if(status_isequal(st, FAILURE)) return NULL;

	st = expect_advance(parser, OPEN_BRACE);
	if(status_isequal(st, FAILURE))
	{
		st = expect_advance(parser, SEMI);
		if(status_isequal(st, FAILURE)) return NULL;

		func_node = malloc(sizeof(astNode));
		if (func_node == NULL) return NULL;

		func_node->type = AST_FUNCTION;
		func_node->nodeData.function.name = func_name;
		func_node->nodeData.function.body = NULL;

		return func_node;
	}

	st = expect_advance(parser, KEYW_RETURN);
	if(status_isequal(st, FAILURE)) return NULL;

	return_node = parse_return(parser);
	if(return_node == NULL) return NULL;

	st = expect_advance(parser, CLOSE_BRACE);
	if(status_isequal(st, FAILURE)) {free_ast(return_node); return NULL;}

	func_node = malloc(sizeof(astNode));
	if(func_node == NULL) {free_ast(return_node); return NULL;}

	func_node->type = AST_FUNCTION;
	func_node->nodeData.function.name = func_name;
	func_node->nodeData.function.body = return_node;

	return func_node;
	//what with all that excessive null check? are we mental?
	//who's we?
}
