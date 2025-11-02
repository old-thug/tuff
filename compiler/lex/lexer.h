#ifndef _TUFF_LEXER_H_
#define _TUFF_LEXER_H_

#include "compiler.h"
#include "lex/loc.h"
#include "lex/token.h"

typedef struct {
    const char *source;
    ModuleId    module_id;

    struct {
	size_t line, prev_line;
	size_t col,  prev_col;
	size_t pos, max;
    };
} Lexer;

inlined Lexer
open_lexer (CompileSession *sess, ModuleId id) {
    Lexer l = {0};
    l.module_id = id;
    l.source    = get_module (sess, id)->source;
    l.line = l.prev_line = l.col = l.prev_col = 1;
    l.max = strlen (l.source);
    return l;
}

Token
lexer_get_token (Lexer *lexer);

#endif // _TUFF_LEXER_H_
