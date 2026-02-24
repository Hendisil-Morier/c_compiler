#ifndef AST_H
#define AST_H

	typedef enum
	{
		AST_PROGRAM,
		AST_FUNCTION,
		AST_RETURN_STMT,
		AST_INT_CONST,
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
				char *name;
				astNode *body;
			} function;
			struct { astNode *expr;} return_stmt;
			struct {long value;} constant;
		} nodeData;
	};


#endif
