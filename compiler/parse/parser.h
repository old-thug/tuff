#ifndef _TUFF_PARSER_H_
#define _TUFF_PARSER_H_

#include "ast/type.h"
#include "compiler.h"
#include "lex/lexer.h"
#include "lex/token.h"
#include "module.h"


typedef struct {
    Lexer lexer;
    CompileSession *sess;
    Allocator *allocator;
    Token prev_token;
    Token current_token;
} Parser;

inlined Parser
open_parser (CompileSession *sess, ModuleId id) {
    Parser p = {0};
    p.sess = sess;
    p.lexer = open_lexer (sess, id);
    p.allocator = &sess->allocator;
    p.current_token = lexer_get_token (&p.lexer);
    return p;
}

bool
parser_parse (Parser *p);

Token
peek_token (Parser *p);

Token
prev_token (Parser *p);

Token
next_token (Parser *p);

bool
expect_token (Parser *p, int id);

bool
match_token (Parser *p, int id);

bool
is_at_end (Parser *p);

bool
is_at_end_or_is (Parser *p, int id);

TypePtr
parse_type (Parser *p);

#endif // _TUFF_PARSER_H_
