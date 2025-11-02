#include "parse/parser.h"
#include "ast/type.h"
#include "lex/lexer.h"
#include "lex/token.h"
#include <stdbool.h>

Token
peek_token (Parser *p) {
    return p->current_token;
}

Token
next_token (Parser *p) {
    p->prev_token = p->current_token;
    p->current_token = lexer_get_token (&p->lexer);
    return p->prev_token;
}

Token
prev_token (Parser *p) {
    return p->prev_token;
}

bool
expect_token (Parser *p, int id) {
    if (!match_token (p, id)) {
	todo ("error");
    }
    next_token (p);
    return true;
}

bool
match_token (Parser *p, int id) {
    return (id == peek_token (p).id);
}

bool
is_at_end (Parser *p) {
    return peek_token (p).id == END_OF_FILE;
}

bool
is_at_end_or_is (Parser *p, int id) {
    return is_at_end (p) || match_token (p, id);
}

static bool
can_begin_type (int tok) {
    switch (tok) {
    case IDENTIFIER:
    case LBRACE:		/* array:   [T; N] */
    case ASTERISK:		/* pointer: *T */
    case AMP:			/* ref:     &T */
    case I8:
    case I16:
    case I32:
    case I64:
    case U8:
    case U16:
    case U32:
    case U64: return true;
    default: return false;
    }
}

TypePtr
parse_type (Parser *p) {
    Token tok = next_token (p);
    if (!can_begin_type (tok.id)) {
	todo ();
    }

    switch (tok.id) {
    case VOID: return make_void_type (p->allocator, tok.locus);
    case I32:  return make_int_type (p->allocator, INTSIGN_Signed, INTSIZE_32, tok.locus);
    default: todo ("parse more type");
    }
}
