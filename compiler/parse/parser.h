#ifndef _TUFF_PARSER_H_
#define _TUFF_PARSER_H_

#include "ast/type.h"
#include "compiler.h"
#include "diag/diag.h"
#include "lex/lexer.h"
#include "lex/token.h"
#include "module.h"

#define parse_error(p, sev, loc, tag, fmt, ...) diagnostic_at (p->diag_collector, \
							       sev,	\
							       loc,	\
							       tag,	\
							       fmt,	\
							       ##__VA_ARGS__)


typedef struct {
    Lexer lexer;
    CompileSession *sess;
    Allocator *allocator;
    DiagnosticCollector *diag_collector;
    Token prev_token;
    Token current_token;
} Parser;

inlined Parser
open_parser (CompileSession *sess, ModuleId id) {
    Parser p = {0};
    p.sess = sess;
    p.lexer = open_lexer (sess, id);
    p.allocator = &sess->allocator;
    p.diag_collector = &sess->diag_collector;
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

void
skip_to (Parser *p, int id);

void
skip_to_after (Parser *p, int id);

void
skip_to_next_type (Parser *p);

TypePtr
parse_type (Parser *p);

#endif // _TUFF_PARSER_H_
