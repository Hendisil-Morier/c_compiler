#ifndef AST_H
#define AST_H

	#include "token_datatype.h"
	typedef struct
	{
		TokenList * list;
		int position;
	} Parser;
	typedef enum
	{
		AST_PROGRAM,
		AST_FUNCTION,
		AST_RETURN_STMT,
		AST_BLOCK,
		AST_INT_CONST,
		AST_STR_LIT,
		AST_EMPT_STMNT,
	} astNodeType;

	typedef struct ASTNode astNode;
	struct ASTNode
	{
		astNodeType type;
		union
		{
			struct
      {
        astNode **function;
        size_t count;
        size_t capacity;
      } program;

			struct
			{
				const char *name; //owned by string interner, do not free. public execution
				astNode *body;
				TokenType return_type;
			} function;
			struct
			{
				size_t count;
				size_t capacity;
				astNode** statements;
			} block;
			struct { astNode *expr;} 	 return_stmt;
			struct {astNode* nullstatement;} empty_statement;
			struct {long value;} 		int_constant;
			struct {const char* value;}   string_lit; //owned by string interner, do not free. public execution
		} nodeData;
	};

	astNode* parse_program(Parser*);

	//a similar ast, but for assembly

	typedef enum
	{
		REG_EAX,
	} Register;

	typedef enum
	{
		NO_OPERAND,
		OPERAND_IMM,
		OPERAND_REG,
	} OperandType;

	typedef struct
	{
		OperandType type;
		union
		{
			int imm;
			Register reg;
		};
	} Operand;

	typedef enum
	{
		INST_MOV,
		INST_RET,
	} InstructType;

	typedef struct
	{
		InstructType type;
		Operand src;
		Operand dest;
	} Instruct;

	typedef struct
	{
		size_t count;
		size_t capacity;
		Instruct intrct_list[];
	} InstructionList;

	typedef struct
	{
		const char* name;
		InstructionList* instructs;
	} ASMfunction;

	typedef struct
	{
		size_t count;
		size_t capacity;
		ASMfunction func_list[];
	} ASMfunctionList;

	typedef struct
	{
		ASMfunctionList* functions;
	} ASMprogram;
#endif
