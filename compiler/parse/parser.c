#include "parse/parser.h"
#include "ast/type.h"
#include "diag/diag.h"
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
	parse_error (p, DIAG_Error, p->current_token.locus,
		     "expected `%s` but got `%s` instead",
		     token_name (id),
		     token_name (p->current_token.id));
	next_token (p);
	return false;
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

void
skip_to (Parser *p, int id) {
    while (!is_at_end_or_is (p, id)) {
	next_token (p);
    }
}

void
skip_to_after (Parser *p, int id) {
    skip_to (p, id);
    next_token (p);
}

static bool
can_begin_type (int tok) {
    switch (tok) {
    case IDENTIFIER:
    case LBRACK:		/* array:   [T; N] */
    case ASTERISK:		/* pointer: *T */
    case AMP:			/* ref:     &T */
    case VOID:
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

static bool
can_begin_expr (int tok) {
    switch (tok) {
    case IDENTIFIER:
    case INT_LITERAL:
    case LPAREN:
    case LBRACE:
    case LBRACK:		/* array:   [T; N] */
    case ASTERISK:		/* pointer: *T */
    case AMP:			/* ref:     &T */
	return true;
    default: return false;
    }
}

void
skip_to_next_type (Parser *p) {
    while (!can_begin_type (peek_token (p).id)) {
	next_token (p);
    }
}

void
skip_to_next_expr (Parser *p) {
    while (!can_begin_expr (peek_token (p).id)) {
	next_token (p);
    }
}

TypePtr
parse_type (Parser *p) {
    Token tok = next_token (p);
    if (!can_begin_type (tok.id)) {
	parse_error (p, DIAG_Error, tok.locus, "expected a type here",
		     "expected one of `identifier`, `[`, `*`, `&`. got `%s`",
		     token_name (tok.id));
	skip_to_next_expr (p);
	return NULL;
    }

    switch (tok.id) {
    case VOID: return make_void_type (p->allocator, tok.locus);
    case I32:  return make_int_type (p->allocator, INTSIGN_Signed, INTSIZE_32, tok.locus);
    default: todo ("parse more type");
    }
}
