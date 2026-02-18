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
        case OPEN_PAREN: return "OPEN_PAREN";
        case CLOSE_PAREN: return "CLOSE_PAREN";
        case OPEN_BRACE: return "OPEN_BRACE";
        case CLOSE_BRACE: return "CLOSE_BRACE";
        case COMMA: return "COMMA";
        case SEMI: return "SEMI";
        case KEYW_INT: return "KEYW_INT";
        case KEYW_RETURN: return "KEYW_RETURN";
        case KEYW_VOID: return "KEYW_VOID";
        case LIT_INT: return "LIT_INT";
        case IDENTIFIER: return "IDENTIFIER";
        case EOF_TOKEN: return "EOF_TOKEN";
        case ERR_TOKEN: return "ERR_TOKEN";
        default: return "UNKNOWN";
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
        printf("%zu", i);
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
