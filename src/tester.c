#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "string_interner.h"
#include "token_datatype.h"
#include "tokenizer.h"

FILE* inFile;
// A simple helper to print token types (you can expand this)
const char* token_type_name(TokenType t) {
    switch(t) {
        // Single-character punctuation
        case OPEN_PAREN:    return "OPEN_PAREN";
        case CLOSE_PAREN:   return "CLOSE_PAREN";
        case OPEN_BRACE:    return "OPEN_BRACE";
        case CLOSE_BRACE:   return "CLOSE_BRACE";
        case OPEN_BRACK:  return "OPEN_BRACK";
        case CLOSE_BRACK: return "CLOSE_BRACK";
        case COMMA:         return "COMMA";
        case SEMI:          return "SEMI";
        case COLON:         return "COLON";          // if you add ':'
        case QUESTION:      return "QUESTION";       // if you add '?'
        case DOT:           return "DOT";            // if you add '.'

        // Operators
        case PLUS_TOKEN:    return "PLUS";
        case MINUS_TOKEN:   return "MINUS";
        case ASTERISK_TOKEN:    return "ASTERISK_TOKEN";           // multiplication or pointer
        case SLASH_TOKEN:   return "SLASH";          // division
        case MODULO: return "PERCENT";        // modulo
        case AMP_TOKEN:     return "AMP";            // bitwise AND or address-of
        case PIPE_TOKEN:    return "PIPE";           // bitwise OR
        case CARET_TOKEN:   return "CARET";          // bitwise XOR
        case TILDE_TOKEN:   return "TILDE";          // bitwise NOT
        case EXCLAIM_TOKEN: return "EXCLAIM";        // logical NOT
        case ASSIGN_TOKEN:  return "ASSIGN";         // '='
        case LT_TOKEN:      return "LT";              // '<'
        case GT_TOKEN:      return "GT";              // '>'

        // Compound assignment and increment/decrement
        case INCREMENT:     return "INCREMENT";      // '++'
        case DECREMENT:     return "DECREMENT";      // '--'
        case ADD_ASSIGN:    return "ADD_ASSIGN";     // '+='
        case SUB_ASSIGN:    return "SUB_ASSIGN";     // '-='
        case MUL_ASSIGN:    return "MUL_ASSIGN";     // '*='
        case DIV_ASSIGN:    return "DIV_ASSIGN";     // '/='
        case MOD_ASSIGN:    return "MOD_ASSIGN";     // '%='
        case AND_ASSIGN:    return "AND_ASSIGN";     // '&='
        case OR_ASSIGN:     return "OR_ASSIGN";      // '|='
        case XOR_ASSIGN:    return "XOR_ASSIGN";     // '^='
        case LEFT_ASSIGN:   return "LEFT_ASSIGN";    // '<<='
        case RIGHT_ASSIGN:  return "RIGHT_ASSIGN";   // '>>='

        // Logical and comparison operators
        case EQUAL_TOKEN:      return "EQUAL";              // '=='
        case NEGATE_TOKEN:      return "NEGATE";              // '!='
        case LE_TOKEN:      return "LE";              // '<='
        case GE_TOKEN:      return "GE";              // '>='
        case LOGICAL_AND:   return "LOGICAL_AND";     // '&&'
        case LOGICAL_OR:    return "LOGICAL_OR";      // '||'
        case LEFT_SHIFT:    return "LEFT_SHIFT";      // '<<'
        case RIGHT_SHIFT:   return "RIGHT_SHIFT";     // '>>'

        // Keywords (C89 first, then others – alphabetical order)
        case KEYW_AUTO:          return "KEYW_AUTO";
        case KEYW_BREAK:         return "KEYW_BREAK";
        case KEYW_CASE:          return "KEYW_CASE";
        case KEYW_CHAR:          return "KEYW_CHAR";
        case KEYW_CONST:         return "KEYW_CONST";
        case KEYW_CONTINUE:      return "KEYW_CONTINUE";
        case KEYW_DEFAULT:       return "KEYW_DEFAULT";
        case KEYW_DO:            return "KEYW_DO";
        case KEYW_DOUBLE:        return "KEYW_DOUBLE";
        case KEYW_ELSE:          return "KEYW_ELSE";
        case KEYW_ENUM:          return "KEYW_ENUM";
        case KEYW_EXTERN:        return "KEYW_EXTERN";
        case KEYW_FLOAT:         return "KEYW_FLOAT";
        case KEYW_FOR:           return "KEYW_FOR";
        case KEYW_GOTO:          return "KEYW_GOTO";
        case KEYW_IF:            return "KEYW_IF";
        case KEYW_INT:           return "KEYW_INT";
        case KEYW_LONG:          return "KEYW_LONG";
        case KEYW_REGISTER:      return "KEYW_REGISTER";
        case KEYW_RETURN:        return "KEYW_RETURN";
        case KEYW_SHORT:         return "KEYW_SHORT";
        case KEYW_SIGNED:        return "KEYW_SIGNED";
        case SIZEOF_OP:        return "SIZEOF_OP";
        case KEYW_STATIC:        return "KEYW_STATIC";
        case KEYW_STRUCT:        return "KEYW_STRUCT";
        case KEYW_SWITCH:        return "KEYW_SWITCH";
        case KEYW_TYPEDEF:       return "KEYW_TYPEDEF";
        case KEYW_UNION:         return "KEYW_UNION";
        case KEYW_UNSIGNED:      return "KEYW_UNSIGNED";
        case KEYW_VOID:          return "KEYW_VOID";
        case KEYW_VOLATILE:      return "KEYW_VOLATILE";
        case KEYW_WHILE:         return "KEYW_WHILE";

        // C99 keywords
        //case KEYW_INLINE:        return "KEYW_INLINE";
        //case KEYW_RESTRICT:      return "KEYW_RESTRICT";
        //case KEYW__BOOL:         return "KEYW__BOOL";
        //case KEYW__COMPLEX:      return "KEYW__COMPLEX";
        //case KEYW__IMAGINARY:    return "KEYW__IMAGINARY";

        // C11 keywords
        //case KEYW__ALIGNAS:      return "KEYW__ALIGNAS";
        //case KEYW__ALIGNOF:      return "KEYW__ALIGNOF";
        //case KEYW__ATOMIC:       return "KEYW__ATOMIC";
        //case KEYW__GENERIC:      return "KEYW__GENERIC";
        //case KEYW__NORETURN:     return "KEYW__NORETURN";
        //case KEYW__STATIC_ASSERT:return "KEYW__STATIC_ASSERT";
        //case KEYW__THREAD_LOCAL: return "KEYW__THREAD_LOCAL";

        // C23 keywords
        //case KEYW__BITINT:       return "KEYW__BITINT";
        //case KEYW__DECIMAL32:    return "KEYW__DECIMAL32";
        //case KEYW__DECIMAL64:    return "KEYW__DECIMAL64";
        //case KEYW__DECIMAL128:   return "KEYW__DECIMAL128";
        //case KEYW_ALIGNAS:       return "KEYW_ALIGNAS";
        //case KEYW_ALIGNOF:       return "KEYW_ALIGNOF";
        //case KEYW_BOOL:          return "KEYW_BOOL";
        //case KEYW_STATIC_ASSERT: return "KEYW_STATIC_ASSERT";
        //case KEYW_THREAD_LOCAL:  return "KEYW_THREAD_LOCAL";
        // Add constexpr, nullptr if you include them

        // Preprocessor directives
        case PRE_INCLUDE:    return "PRE_INCLUDE";
        case PRE_DEFINE:     return "PRE_DEFINE";
        case PRE_IF:         return "PRE_IF";
        case PRE_IFDEF:      return "PRE_IFDEF";
        case PRE_IFNDEF:     return "PRE_IFNDEF";
        case PRE_ELSE:       return "PRE_ELSE";
        case PRE_ELIF:       return "PRE_ELIF";
        case PRE_ENDIF:      return "PRE_ENDIF";
        case PRE_PRAGMA:     return "PRE_PRAGMA";
        case PRE_ERROR:      return "PRE_ERROR";
        case PRE_LINE:       return "PRE_LINE";
        case PRE_UNDEF:      return "PRE_UNDEF";

        // Literals
        case LIT_INT:        return "LIT_INT";
        //case LIT_STR:        return "LIT_STR";
        //case LIT_CHAR:       return "LIT_CHAR";      // if you add character literals
        //case LIT_FLOAT:      return "LIT_FLOAT";     // if you add floating point

        // Identifiers and special tokens
        case IDENTIFIER:     return "IDENTIFIER";
        case EOF_TOKEN:      return "EOF_TOKEN";
        case ERR_TOKEN:      return "ERR_TOKEN";

        default:             return "UNKNOWN";
    }
}

int main(int argc, const char* argv[])
{
	ident_intern = new_interner();
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file
    FILE* f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    // Allocate buffer (size + 1 for EOF sentinel)
    char_f* buffer = malloc((size + 1) * sizeof(char_f));
    if (!buffer) {
        perror("malloc failed");
        fclose(f);
        return 1;
    }

    for(size_t i = 0; i < size; i++)
    {
    	buffer[i] = fgetc(f);
    }

    // Add EOF sentinel at the end
    buffer[size] = EOF;

    // Initialize lexer
    Lexer lex;
    lex.cur = buffer;
    lex.line = 1;  // will be overwritten by get_token, but set anyway

    // Tokenize
    TokenList* tokens = get_token(&lex);
    if (!tokens) {
        fprintf(stderr, "get_token failed\n");
        free(buffer);
        return 1;
    }

    // Print tokens (for testing)
    for (size_t i = 0; i < tokens->length; i++) {
        Token t = tokens->tklist[i];
        printf("Line %d: %s", t.line, token_type_name(t.type));
        if (t.type == LIT_INT) {
            printf(" (%ld)", t.data.int_val);
        } else if (t.type == IDENTIFIER) {
            printf(" (%s)", t.data.iden_name ? t.data.iden_name : "?");
        }
        printf("\n");
    }

    // Clean up
    list_destroy(tokens);
    destroy_iden_intern();
    free(buffer);
    return 0;
}
