#include "array.h"
#include "ast/node_expr.h"
#include "ast/node_kind.h"
#include "ast/type.h"
#include "def.h"
#include "diag/diag.h"
#include "lex/loc.h"
#include "lex/token.h"
#include "parse/parser.h"
#include "ast/node.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef enum {
    ASSOC_Left,
    ASSOC_Right
} Associativity;

typedef NodePtr (*ParseInfix)(Parser *parser, NodePtr left);
typedef NodePtr (*ParsePrefix)(Parser *parser);

typedef struct {
    ParsePrefix prefix;
    ParseInfix  infix;
    Precedence   precedence;
    Associativity associativity;
} ParseRule;

NodePtr
parse_primary(Parser *p);
NodePtr
parse_unary(Parser *p);
NodePtr
parse_block(Parser *p);
NodePtr
parse_if_expr(Parser *p);
NodePtr
parse_binop(Parser *p, NodePtr left);
NodePtr
parse_assign(Parser *p, NodePtr reciever);
NodePtr
parse_function_call(Parser *p, NodePtr left);
NodePtr
parse_group (Parser *p, NodePtr first);

static const
ParseRule parse_rules[] = {
    [LPAREN]     =  {parse_primary, parse_function_call, PREC_Postfix, ASSOC_Left},
    [LBRACE]     =  {parse_block, NULL, PREC_Primary, ASSOC_Left},
    [IF]     =  {parse_if_expr, NULL, PREC_Primary, ASSOC_Left},
    [LBRACK]     =  {NULL, NULL, PREC_Postfix, ASSOC_Left},
    [COMMA]      =  {NULL, parse_group, PREC_Comma, ASSOC_Left},
    [EQUAL]      =  {NULL, parse_binop, PREC_Assign, ASSOC_Right},
    [PLUS]       =  {parse_unary, parse_binop, PREC_Additive, ASSOC_Left},
    [MINUS]      =  {parse_unary, parse_binop, PREC_Additive, ASSOC_Left},
    [ASTERISK]   =  {NULL, parse_binop, PREC_Multiplicative, ASSOC_Left},
    [DIV]        =  {NULL, parse_binop, PREC_Multiplicative, ASSOC_Left},
    [IDENTIFIER] =  {parse_primary, NULL, PREC_Primary, ASSOC_Left},
    [INT_LITERAL] =  {parse_primary, NULL, PREC_Primary, ASSOC_Left},
    [ASSIGN]     =      {NULL, parse_assign, PREC_Assign, ASSOC_Right},
    [COLON]      =      {NULL, parse_assign, PREC_Assign, ASSOC_Right},
};

NodePtr
parse_expr(Parser *p, int min_prec) {
    Token token = peek_token(p);
    ParseRule rule = parse_rules[token.id];

    if (rule.prefix == NULL) {
	if (token.id == SEMICOLON) {
	    Diagnostic *diag = parse_error(p, DIAG_Error, token.locus,
					   "expected start of primary expression",
					   "unexpected extra `;`");
	    diag_simple_note(diag, "did you mean to remove one semicolon?");
	} else {
	    parse_error (p, DIAG_Error, token.locus,
			 "expected start of primary expression",
			 "unexpected token");
	}
	next_token (p);
	return make_error_node (p->allocator, token.locus);
    }

    NodePtr left = rule.prefix (p);
    while (true) {
        Token next = peek_token (p);
	/* Function calls should parse arguments seperately and not as a group. */
	if (next.id == COMMA && p->context == PARSE_FunctionArgs) break;
	
	ParseRule next_rule = parse_rules[next.id];
	
        if (next_rule.precedence < min_prec ||
            (next_rule.precedence == min_prec && next_rule.associativity == ASSOC_Left)) {
            break;
        }

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

enum BinOp
binop_from_token (int id) {
    switch (id) {
    case PLUS: return BINOP_Add;
    case MINUS:  return BINOP_Sub;
    case ASTERISK: return BINOP_Mul;
    case DIV: return BINOP_Div;
    case EQUAL_EQUAL: return BINOP_Equal;
    default: todo ();
    }
}

NodePtr
parse_primary (Parser *p) {
    Token tok = next_token (p);
    NodePtr node;
    switch (tok.id) {
    case IDENTIFIER: {
	node = make_node (p->allocator, NODE_Ident, tok.locus);
	node->identifier.locus = tok.locus;
	node->identifier.value = tok.str;
    } break;
    case INT_LITERAL: {
	const char *str = sv_get (tok.str);
	int64_t value = strtol (str, NULL, 10);
	node = make_int_expr (p->allocator, value, tok.locus);
	free ((char*)str);
    } break;
    case LPAREN: {
	// Temporarily reset context so inner expression can group (e.g. (a, b) becomes GroupExpr)
	ParseContext prev = p->context;
	p->context = PARSE_Default;
	node = parse_expr(p, 0);
	p->context = prev;
	expect_token(p, RPAREN);
    } break;
    default: todo ("parse primary expression: `%s`", token_name (tok.id));
    }
    return node;
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
	} else if (stmt && may_consume_semicolon (stmt)) next_token (p);    /* consume semicolon */
	append_node_to_block (block, stmt);
    }
    tok = peek_token (p);
    expect_token (p, RBRACE);
    set_block_end_locus (block, tok.locus);
    return block;
}

NodePtr
parse_binop (Parser *p, NodePtr left) {
    Token op = next_token (p);
    NodePtr right = parse_expr (p, 0);
    switch (op.id) {
    case EQUAL: {
	return make_assign_expr (p->allocator, op.locus, left, right);
    } break;
    case PLUS:
    case MINUS:
    case ASTERISK:
    case DIV:
    case EQUAL_EQUAL: {
	enum BinOp binop = binop_from_token (op.id);
	return make_binop_expr (p->allocator, op.locus, binop, left, right);
    } break;
    default: todo ("%s", token_name (op.id));
    }
}

NodePtr
parse_function_call(Parser *p, NodePtr left) {
    ArgumentList arguments = {0};
    next_token (p);
    ParseContext prev = p->context;
    p->context = PARSE_FunctionArgs;
    while (!is_at_end_or_is (p, RPAREN)) {
	NodePtr argument = parse_expr (p, 0);
	arr_push (&arguments, argument);

	if (match_token (p, COMMA)) {
	    Token comma = next_token (p);
	    if (match_token (p, RPAREN)) {
		parse_error (p, DIAG_Error, comma.locus,
			     "consider removing this `,` to complete the argument list",
			     "expected an expression");
		break;
	    }
	}
    }
    p->context = prev;
    expect_token (p, RPAREN);
    return make_function_call_expr (p->allocator, left->primary_locus,
				    left, arguments);
}

NodePtr
parse_assign (Parser *p, NodePtr receiver) {
    Token tok = next_token (p);
    if (tok.id == ASSIGN) { /* receiver `:=` expression */
	Mutability mutability = MUTABILITY_Mutable;
	TypePtr type = make_pending_type (p->allocator, receiver->primary_locus);
	NodePtr expression = parse_expr (p, 0);
	return make_declare_expr (p->allocator, tok.locus, mutability, receiver, expression, type);
    } else if (tok.id == COLON) { /* receiver `:` type `=` expression */
	TypePtr type = parse_type (p);
	Token op = peek_token (p);
	if (op.id != COLON && op.id != EQUAL) {
	    todo ("raise error");
	}
	next_token (p);
	Mutability mutability = (op.id == EQUAL)? MUTABILITY_Mutable: MUTABILITY_Immutable;
	NodePtr expression = parse_expr (p, 0);
	return make_declare_expr (p->allocator, tok.locus, mutability, receiver, expression, type);
    }
    todo ();
}

NodePtr
parse_group (Parser *p, NodePtr first) {
    if (p->context == PARSE_FunctionArgs) return first;
    Location begin = first->primary_locus;
    Location end = begin;
    NodeList members = {0};
    arr_push (&members, first);
    for (;;) {
	if (match_token (p, COMMA)) {
	    next_token (p);
	    NodePtr member = parse_expr (p, 0);
	    arr_push (&members, member);
	    end = member->primary_locus;
	} else break;
    }
    return make_group_expr (p->allocator, begin, end, members);
}


NodePtr
parse_if_expr(Parser *p) {
    Token if_tok = next_token (p);
    NodePtr condition = parse_expr (p, 0);
    NodePtr then_block = parse_block(p);
    NodePtr else_block = NULL;
    if (match_token(p, ELSE)) {
        next_token(p);
	TODO (old-thug, "parse `else if` chains");
	else_block = parse_block(p);
    }

    return make_if_expr(p->allocator, if_tok.locus, condition, then_block, else_block);
}
