#include "ast.h"
#include "error.h"
#include <stdlib.h>

Status gen_return(astNode* return_statement, ASMfunction* func);

static Status list_expand(InstructionList** list_ptr, size_t new_cap)
{
	InstructionList* list = NULL;
	if (!list_ptr || !(list = *list_ptr)) return FAILURE;
	if(new_cap <= list->capacity) return NOT_EXECUTE;

	InstructionList* temp = realloc(list,sizeof(InstructionList) + new_cap*sizeof(Instruct));
	if(!temp) return FAILURE;

	temp->capacity = new_cap;
	*list_ptr = temp;

	return SUCCESS;
}

static Status list_append_instr(InstructionList** list_ptr, Instruct* instruct)
{
	InstructionList* list = NULL;
	if (!list_ptr || !instruct || !(list = *list_ptr)) return FAILURE;

	if (list->count >= list->capacity)
	{
		size_t new_cap = list->capacity * 3/2 + 1;
		Status st = list_expand(list_ptr, new_cap);
		if (status_isequal(st, FAILURE)) return FAILURE;
		list = *list_ptr;
	}
	list->intrct_list[list->count++] = *instruct;
	return SUCCESS;
}

InstructionList* list_init_instr(size_t capacity)
{
	InstructionList* list = malloc(sizeof(InstructionList) + capacity*sizeof(Instruct));
	if (!list) return NULL;

	list->capacity = capacity;
	list->count = 0;

	return list;
}

void free_instr_list(InstructionList* list)
{
	if (!list) return;
	free(list);
}

void free_asm_func(ASMfunction* func)
{
	if (!func) return;
	free_instr_list(func->instructs);
	free(func);
}

Status gen_return(astNode* return_stmnt, ASMfunction* func)
{
	if (!return_stmnt || !func) return FAILURE;

	astNode* expr = return_stmnt->nodeData.return_stmt.expr;
	if (!expr) return FAILURE;

	Instruct mov = {
		.type = INST_MOV,
		.src = {.type = OPERAND_IMM, .imm = expr->nodeData.int_constant.value},
		.dest = {.type = OPERAND_REG, .reg = REG_EAX}
	};

	Instruct ret = {
		.type = INST_RET,
		.dest = {.type = NO_OPERAND},
		.src = {.type = NO_OPERAND}
	};

	if(status_isequal(list_append_instr(&func->instructs, &mov), FAILURE)) return FAILURE;
	if(status_isequal(list_append_instr(&func->instructs, &ret), FAILURE)) return FAILURE;

	return SUCCESS;
}

ASMfunction* asm_func_init(astNode* funcNode)
{
	if (!funcNode) return NULL;

	ASMfunction* asm_func = malloc(sizeof(ASMfunction));
	if (!asm_func) return NULL;

	asm_func->name = funcNode->nodeData.function.name;

	asm_func->instructs = list_init_instr(8);
	if (!asm_func->instructs) {free(asm_func); return NULL;}

	astNode* body = funcNode->nodeData.function.body;
	if (!body) return asm_func;

	for (size_t i = 0; i < body->nodeData.block.count; i++)
	{
		astNode* statement = body->nodeData.block.statements[i];
		if(!statement)
		{
			free_asm_func(asm_func);
			return NULL;
		}
		switch(statement->type)
		{
		case AST_RETURN_STMT:
			if(status_isequal(gen_return(statement, asm_func), FAILURE))
			{
				free_asm_func(asm_func);
				return NULL;
			}
			break;
		default:
			break;
		}
	}

	return asm_func;
}
