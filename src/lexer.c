#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "token_datatype.h"
#include "tokenizer.h"
#include "string_interner.h"

// insanely unsafe.
// for convinient only. these thing have only one use and will fuck the code up in other places.
#define iden_condition(x) (x >= 'A' && x <= 'Z' || x >= 'a' && x <= 'z' || x == '_')
#define is_single(c) (cur_char == c)
#define IDENT_BUFFER_SIZE 256

extern FILE* inFile;

const LookupList keyword_list[] =
	{
 		{"auto",        KEYW_AUTO},
	    {"break",       KEYW_BREAK},
	    {"case",        KEYW_CASE},
	    {"char",        KEYW_CHAR},
	    {"const",       KEYW_CONST},
	    {"continue",    KEYW_CONTINUE},
	    {"default",     KEYW_DEFAULT},
	    {"do",          KEYW_DO},
	    {"double",      KEYW_DOUBLE},
	    {"else",        KEYW_ELSE},
	    {"enum",        KEYW_ENUM},
	    {"extern",      KEYW_EXTERN},
	    {"float",       KEYW_FLOAT},
	    {"for",         KEYW_FOR},
	    {"goto",        KEYW_GOTO},
	    {"if",          KEYW_IF},
	    {"int",         KEYW_INT},
	    {"long",        KEYW_LONG},
	    {"register",    KEYW_REGISTER},
	    {"return",      KEYW_RETURN},
	    {"short",       KEYW_SHORT},
	    {"signed",      KEYW_SIGNED},
	    {"sizeof",      SIZEOF_OP},
	    {"static",      KEYW_STATIC},
	    {"struct",      KEYW_STRUCT},
	    {"switch",      KEYW_SWITCH},
	    {"typedef",     KEYW_TYPEDEF},
	    {"union",       KEYW_UNION},
	    {"unsigned",    KEYW_UNSIGNED},
	    {"void",        KEYW_VOID},
	    {"volatile",    KEYW_VOLATILE},
	    {"while",       KEYW_WHILE},
	};

void skip_whitespace(Lexer* lex)
{
	while (1)
	{
		char_f c = *(lex->cur);
		if(c==EOF) break;
		if(c=='\n') lex->line++;
		if(c == ' ' || c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v')
		{
			(lex->cur++);
		}
		else break;
	}
}

Status read_file(const char* path)
{
	FILE* file = fopen(path, "rb");
	if(file == NULL) return FAILURE;

	inFile = file;
	return SUCCESS;
}

bool is_keywords(const char* buffer, Token* new_token)
{
	if(buffer == NULL || new_token == NULL) return false;

	for (size_t i = 0; i < (sizeof(keyword_list)/sizeof(keyword_list[0])); i++)
	{
		if (strcmp(buffer, keyword_list[i].c) == 0)
		{
			new_token->type = keyword_list[i].type;
			return true;
		}
	}
	return false;
}

void is_operator(const char cur_char, Token* new_token)
{
//unfinished
}

TokenList* get_token(Lexer* lex)
{

	TokenList* new_list = list_init(10);
	if (new_list == NULL) return NULL;
	Status st = FAILURE;
	char_f cur_char = *lex->cur;

	while(1)
	{
		skip_whitespace(lex);
		cur_char = *lex->cur;
		Token new_token = {0};
		if(cur_char == EOF) break;

		if (is_single('('))
		{
			lex->cur++; new_token.type = OPEN_PAREN;
		}
		else if (is_single(')'))
		{
			lex->cur++; new_token.type = CLOSE_PAREN;
		}
		else if (is_single('{'))
		{
			lex->cur++;	new_token.type = OPEN_BRACE;
		}
		else if (is_single('}'))
		{
			lex->cur++;	new_token.type = CLOSE_BRACE;
		}
		else if (is_single(','))
		{
			lex->cur++;	new_token.type = COMMA;
		}
		else if (is_single(';'))
		{
			lex->cur++;	new_token.type = SEMI;
		}
		else if (is_single('['))
		{
			lex->cur++; new_token.type = OPEN_BRACK;
		}
		else if (is_single(']'))
		{
			lex->cur++; new_token.type = CLOSE_BRACK;
		}
		else if (is_single('.'))
		{
			lex->cur++; new_token.type = DOT;
		}
		else if (is_single('/'))
		{
			if(*(lex->cur + 1) == '/')
			{
				while(*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
				continue;
			}
			else if (*(lex->cur + 1) == '*')
			{
				lex->cur+=2;
				while(*lex->cur != EOF)
				{
					if(*lex->cur == '*' && *(lex->cur + 1) == '/')
					{
						lex->cur+=2; break;
					}
					lex->cur++;
				}
				if (*lex->cur==EOF)
				{
					new_token.type = ERR_TOKEN;
				}
				continue;
			}
			else if (*(lex->cur + 1) == '=')
			{
				new_token.type = DIV_ASSIGN;
				lex->cur += 2;
			}
			else
			{
				new_token.type = SLASH_TOKEN;
				lex->cur++;
			}
		}
		else if (is_single('='))
		{
			if (*(lex->cur + 1) == '=')
			{
				new_token.type = EQUAL_TOKEN;
				lex->cur += 2;
			}
			else
			{
				new_token.type = ASSIGN_TOKEN;
				lex->cur++;
			}
		}
		else if(is_single('+'))
		{
			if (*(lex->cur + 1) == '=')
			{
				new_token.type = ADD_ASSIGN;
				lex->cur += 2;
			}
			else if (*(lex->cur + 1) == '+')
			{
				new_token.type = INCREMENT;
				lex->cur += 2;
			}
			else
			{
				new_token.type = PLUS_TOKEN;
				lex->cur++;
			}
		}
		else if (is_single('-'))
		{
			if (*(lex->cur + 1) == '-')
			{
				new_token.type = DECREMENT;
				lex->cur += 2;
			}
			else if (*(lex->cur + 1) == '=')
			{
				new_token.type = SUB_ASSIGN;
				lex->cur+=2;
			}
			else if (*(lex->cur + 1) == '>')
			{
				new_token.type = ARROW;
				lex->cur+=2;
			}
			else
			{
				new_token.type = MINUS_TOKEN;
				lex->cur++;
			}
		}
		else if (is_single('<'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = LE_TOKEN;
		        lex->cur += 2;
		    }
		    else if (*(lex->cur + 1) == '<')
		    {
		        // left shift
		        if (*(lex->cur + 2) == '=')
		        {
		            new_token.type = LEFT_ASSIGN;
		            lex->cur += 3;
		        }
		        else
		        {
		            new_token.type = LEFT_SHIFT;
		            lex->cur += 2;
		        }
		    }
		    else
		    {
		        new_token.type = LT_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('>'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = GE_TOKEN;
		        lex->cur += 2;
		    }
		    else if (*(lex->cur + 1) == '>')
		    {
		        // left shift
		        if (*(lex->cur + 2) == '=')
		        {
		            new_token.type = RIGHT_ASSIGN;
		            lex->cur += 3;
		        }
		        else
		        {
		            new_token.type = RIGHT_SHIFT;
		            lex->cur += 2;
		        }
		    }
		    else
		    {
		        new_token.type = GT_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('*'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = MUL_ASSIGN;
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = ASTERISK_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('%'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = MOD_ASSIGN;
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = MODULO;
		        lex->cur++;
		    }
		}
		else if (is_single('&'))
		{
		    if (*(lex->cur + 1) == '&')
		    {
		        new_token.type = LOGICAL_AND;
		        lex->cur += 2;
		    }
		    else if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = AND_ASSIGN;
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = AMP_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('|'))
		{
		    if (*(lex->cur + 1) == '|')
		    {
		        new_token.type = LOGICAL_OR;
		        lex->cur += 2;
		    }
		    else if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = OR_ASSIGN;
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = PIPE_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('^'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = XOR_ASSIGN;
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = CARET_TOKEN;
		        lex->cur++;
		    }
		}
		else if (is_single('~'))
		{
		    new_token.type = TILDE_TOKEN;
		    lex->cur++;
		}
		else if (is_single('!'))
		{
		    if (*(lex->cur + 1) == '=')
		    {
		        new_token.type = NEQUAL_TOKEN;   // !=
		        lex->cur += 2;
		    }
		    else
		    {
		        new_token.type = EXCLAIM_TOKEN;  // !
		        lex->cur++;
		    }
		}
		else if (is_single('?'))
		{
		    new_token.type = QUESTION;
		    lex->cur++;
		}
		else if (is_single(':'))
		{
		    new_token.type = COLON;
		    lex->cur++;
		}
		else if (is_single('"'))
		{
		    // Consume the opening quote
		    lex->cur++;

		    // Dynamically growable buffer for the string content
		    size_t cap = 32;
		    size_t len = 0;
		    char *buffer = malloc(cap);
			if (!buffer) {
		        // Out of memory – set error token and abort
		        new_token.type = ERR_TOKEN;
		        // Optionally, skip to closing quote or newline to recover
		        while (*lex->cur != '"' && *lex->cur != '\n' && *lex->cur != EOF)
		            lex->cur++;
		        if (*lex->cur == '"') lex->cur++;
		    }
			else
			{
		        // Read until closing quote or newline/EOF
		        while (*lex->cur != '"' && *lex->cur != '\n' && *lex->cur != EOF)
				{
		            if (len + 1 >= cap)
					{
		                cap *= 2;
		                char *new_buf = realloc(buffer, cap);
		                if (!new_buf)
						{
		                    free(buffer);
		                    buffer = NULL;
		                    break;
		                }
		                buffer = new_buf;
		            }

					if (*lex->cur == '\\')
					{
		                // Escape sequence – simplified handling
		                lex->cur++;
		                switch (*lex->cur)
						{
		                    case 'n': buffer[len++] = '\n'; break;
							case 'f': buffer[len++] = '\f'; break;
							case 'v': buffer[len++] = '\v'; break;
		                    case 't': buffer[len++] = '\t'; break;
		                    case '"': buffer[len++] = '"'; break;
		                    case '\\': buffer[len++] = '\\'; break;
		                    // Add more escapes
		                    default: buffer[len++] = *lex->cur; break;
		                }
		                lex->cur++;
		            }
					else buffer[len++] = *lex->cur++;
		        }
				if (buffer) {
		            buffer[len] = '\0';
		            // Check if we terminated correctly
		            if (*lex->cur != '"') {
		                // Unterminated string
		                free(buffer);
		                new_token.type = ERR_TOKEN;
		                // Skip to newline or EOF to recover
		                while (*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
		            } else {
		                // Consume closing quote
		                lex->cur++;
		                // Intern the string
		                const char* interned = stringIntern_find(str_literal_intern, buffer);
		                if (!interned) {
		                    intern_append(str_literal_intern, buffer);
		                    interned = str_literal_intern->string[str_literal_intern->length - 1];
		                }
		                new_token.type = LIT_STR;
		                new_token.data.str_val = (char*)interned;
		                free(buffer); // buffer no longer needed
		            }
		        }
				else
				{
		            // Allocation failed
		            new_token.type = ERR_TOKEN;
		            // Skip to closing quote or newline
		            while (*lex->cur != '"' && *lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
		            if (*lex->cur == '"') lex->cur++;
		        }
		    }
		}
		else if (is_single('\''))
		{
			lex->cur++; // consume opening quote
			if (*lex->cur == '\'' || *lex->cur == '\n' || *lex->cur == EOF)
			{
				// empty or unclosed character literal
				new_token.type = ERR_TOKEN;
				while (*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
			}
			else
			{
				char value;
				if (*lex->cur == '\\')
				{
				    lex->cur++; // consume backslash
					if (*lex->cur == EOF || *lex->cur == '\n')
					{
				        new_token.type = ERR_TOKEN;
				        while (*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
				    }
					else
					{
				        switch (*lex->cur)
						{
				            case 'n':  value = '\n'; break;
				            case 't':  value = '\t'; break;
				            case 'r':  value = '\r'; break;
				            case '\\': value = '\\'; break;
				            case '\'': value = '\''; break;
				            case '"':  value = '"';  break;
				            default:   value = *lex->cur; break;
				        }
				        lex->cur++; // consume the escaped character
				        // expect closing quote
						if (*lex->cur == '\'')
						{
				            lex->cur++;
				            new_token.type = LIT_CHAR;
				            new_token.data.char_val = value;
				        }
						else
						{
				            new_token.type = ERR_TOKEN;
				            while (*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
				        }
				    }
				}
				else
				{
				    value = *lex->cur;
				    lex->cur++;
					if (*lex->cur == '\'')
					{
				        lex->cur++;
				        new_token.type = LIT_CHAR;
				        new_token.data.char_val = value;
				    }
					else
					{
				        new_token.type = ERR_TOKEN;
				        while (*lex->cur != '\n' && *lex->cur != EOF) lex->cur++;
				    }
				}
			}
		}
		else if (is_single('#'))
		{
			char buffer[IDENT_BUFFER_SIZE];
     		size_t i = 0;
       		lex->cur++;

         	while(*lex->cur == ' ' || *lex->cur == '\t')
          		lex->cur++;

       		// Read characters while they are valid identifier chars
         	while (i < IDENT_BUFFER_SIZE - 1 && (iden_condition(*lex->cur) || (*lex->cur >= '0' && *lex->cur <= '9')))
			{
		       buffer[i++] = *lex->cur;
		       lex->cur++;
			}
          	buffer[i] = '\0';

           	// Check if it's a directive
			if (strcmp(buffer, "include") == 0)
		    	new_token.type = PRE_INCLUDE;
			else if (strcmp(buffer, "define") == 0)
		    	new_token.type = PRE_DEFINE;
			else if (strcmp(buffer, "if") == 0)
				new_token.type = PRE_IF;
			else if (strcmp(buffer, "endif") == 0)
				new_token.type = PRE_ENDIF;
			else if (strcmp(buffer, "ifndef") == 0)
				new_token.type = PRE_IFNDEF;
			else
				new_token.type = ERR_TOKEN;
		}
		else if (cur_char >= '0' && cur_char <= '9')
		{
			int value = 0;
			while(*lex->cur >= '0' && *lex->cur <= '9')
			{
				value = value*10 + (*lex->cur - '0');
				lex->cur++;
			}
			new_token.type = LIT_INT;
			new_token.data.int_val  = value;
		}
		else if (iden_condition(cur_char))
		{
    		char buffer[IDENT_BUFFER_SIZE];
     		size_t i = 0;

		    // Read characters while they are valid identifier chars
		    while (i < IDENT_BUFFER_SIZE - 1 && (iden_condition(*lex->cur) || (*lex->cur >= '0' && *lex->cur <= '9')))
			{
		        buffer[i++] = *lex->cur;
		        lex->cur++;
		    }
		    buffer[i] = '\0';

		    // Check if it's a keyword
			if (is_keywords(buffer, &new_token))
			{;}
			else
		    {
		        // It's an identifier – intern it
		        const char* interned = stringIntern_find(ident_intern, buffer);
		        if (!interned) {
		            // Not yet interned: add it
		            intern_append(ident_intern, buffer);
		            interned = ident_intern->string[ident_intern->length - 1]; // pointer to the new copy
		        }
		        new_token.type = IDENTIFIER;
		        new_token.data.iden_name = (char*)interned;   // cast away const – but the interner owns it
		    }
		}
		else
		{
			lex->cur++;
			new_token.type = ERR_TOKEN;
			//continue;
		}

		new_token.line = lex->line;
		st = list_append(&new_list, &new_token);
		if (status_isequal(st, FAILURE))
			{list_destroy(new_list); return NULL;}
	}

	Token eof_tok = {.type = EOF_TOKEN};
	list_append(&new_list, &eof_tok);
	return new_list;
}
