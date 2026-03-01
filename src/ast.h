#ifndef AST_H
#define AST_H

	#include<stddef.h>
	typedef enum
	{
		AST_PROGRAM,
		AST_FUNCTION,
		AST_RETURN_STMT,
		AST_BLOCK,
		AST_INT_CONST,
		AST_STR_LIT
	} astNodeType;

	typedef struct ASTNode astNode;
	struct ASTNode
	{
		astNodeType type;
		union
		{
			struct {astNode *function;} program;
			struct
			{
				const char *name; //owned by string interner, do not free. public execution
				astNode *body;
			} function;
			struct
			{
				size_t count;
				size_t capacity;
				astNode** statements;
			} block;
			struct { astNode *expr;} 	 return_stmt;
			struct {long value;} 		int_constant;
			struct {const char* value;}   string_lit; //owned by string interner, do not free. public execution
		} nodeData;
	};


#endif
