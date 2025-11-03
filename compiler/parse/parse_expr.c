#include "ast/node_expr.h"
#include "ast/node_kind.h"
#include "diag/diag.h"
#include "lex/loc.h"
#include "lex/token.h"
#include "parse/parser.h"
#include "ast/node.h"
#include <stdio.h>

typedef enum {
    PREC_None,
    PREC_Comma,             // ,
    PREC_Assign,            // =, +=, -=, etc.
    PREC_Ternary,           // ?:
    PREC_LogicalOr,         // ||
    PREC_LogicalAnd,        // &&
    PREC_BitOr,             // |
    PREC_BitXor,            // ^
    PREC_BitAnd,            // &
    PREC_Equality,          // ==, !=
    PREC_Relational,        // <, <=, >, >=
    PREC_Shift,             // <<, >>
    PREC_Additive,          // +, -
    PREC_Multiplicative,    // *, /, %
    PREC_Unary,             // !, ~, +, -, *
    PREC_Postfix,           // ++, --, (), [], .
    PREC_Primary            // literals, identifiers
} Precedence;

typedef NodePtr (*ParseInfix)(Parser *parser, NodePtr left);
typedef NodePtr (*ParsePrefix)(Parser *parser);

typedef struct {
    ParsePrefix prefix;
    ParseInfix  infix;
    Precedence   precedence;
} ParseRule;

NodePtr
parse_primary(Parser *parser);
NodePtr
parse_unary(Parser *parser);
NodePtr
parse_block(Parser *parser);
NodePtr
parse_binop(Parser *parser, NodePtr left);
NodePtr
parse_function_call(Parser *parser, NodePtr left);

static const
ParseRule parse_rules[] = {
    [LPAREN]     =	{parse_primary, parse_function_call, PREC_Postfix},
    [LBRACE]     =	{parse_block, NULL, PREC_Primary},
    [LBRACK]     =	{NULL, NULL, PREC_Postfix},
    [COMMA]      =	{NULL, NULL, PREC_Comma},
    [EQUAL]      =	{NULL, parse_binop, PREC_Assign},
    [PLUS]       =	{parse_unary, parse_binop, PREC_Additive},
    [MINUS]      =	{parse_unary, parse_binop, PREC_Additive},
    [ASTERISK]   =	{NULL, parse_binop, PREC_Multiplicative},
    [DIV]        =	{NULL, parse_binop, PREC_Multiplicative},
    [IDENTIFIER] =	{parse_primary, NULL, PREC_Primary},
};

NodePtr
parse_expr(Parser *p, int min_prec) {
    Token token = peek_token(p);
    ParseRule rule = parse_rules[token.id];

    if (rule.prefix == NULL) {
	parse_error (p, DIAG_Error, token.locus, NULL,
		     "expected start of primary expression");
	return NULL;
    }

    NodePtr left = rule.prefix (p);
    while (true) {
        Token next = peek_token (p);
        ParseRule next_rule = parse_rules[next.id];

        if (next_rule.precedence <= min_prec) break;

        if (next_rule.infix) {
            left = next_rule.infix(p, left);
	} else {
            break;
        }
    }

    return left;
}

bool
may_consume_semicolon (NodePtr node) {
    switch (node->kind) {
    case NODE_If:
    case NODE_While:
    case NODE_For:
    case NODE_Block:
	return false;
    default: return true;
    }
}

NodePtr
parse_primary (Parser *p) {
    Token tok = next_token (p);
    NodePtr node;
    if (tok.id == IDENTIFIER) {
	node = make_node (p->allocator, NODE_Ident, tok.locus);
	node->identifier.locus = tok.locus;
	node->identifier.value = tok.str;
	return node;
    }
    todo ("parse primary expression");
}

NodePtr
parse_unary (Parser *p) { todo("parse unary expression"); }

NodePtr
parse_block (Parser *p) {
    Token tok = next_token (p);
    NodePtr block = make_block_expr (p->allocator, tok.locus);
    while (!is_at_end_or_is (p, RBRACE)) {
	NodePtr stmt = parse_expr (p, 0);
	// Implicit return?
	if (!match_token (p, SEMICOLON) && match_token (p, RBRACE)) {
	    stmt = make_return_expr (p->allocator, stmt, stmt->primary_locus);
	}
	append_node_to_block (block, stmt);
    }
    tok = peek_token (p);
    expect_token (p, RBRACE);
    set_block_end_locus (block, tok.locus);
    return block;
}

NodePtr
parse_binop (Parser *p, NodePtr left) { todo (); }

NodePtr
parse_function_call(Parser *p, NodePtr left) { todo (); }
