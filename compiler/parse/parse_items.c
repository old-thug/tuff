#include "ast/node.h"
#include "ast/node_expr.h"
#include "ast/node_item.h"
#include "ast/node_kind.h"
#include "lex/loc.h"
#include "lex/token.h"
#include "parse/parser.h"

NodePtr
parse_function (Parser *p);

bool
parse_items (Parser *p);

bool
parser_parse (Parser *p) {
    return parse_items (p);
}

bool
parse_items (Parser *p) {
    while (!is_at_end (p)) {
	switch (peek_token (p).id) {
	case FUNC: parse_function (p); break;
	default: todo ();
	}
    } 
    return false;
}

bool
parse_identifier (Parser *p, IdentifierExpr *ident) {
    if (!match_token (p, IDENTIFIER)) {
	return false;
    }
    Token tok = next_token (p);
    ident->value = tok.str;
    ident->locus = tok.locus;
    return true;
}

bool
parse_function_parameters (Parser *p, FunctionParameterList *params) {
    expect_token (p, LPAREN);

    while (!is_at_end_or_is (p, RPAREN)) {
	FunctionParameter param;
	if (!parse_identifier (p, &param.name)) todo ();
	expect_token (p, COLON);
	param.type = parse_type (p);

	if (match_token (p, EQUAL)) {
	    todo ("parse default parameter value");
	}

	arr_push (params, param);
	if (match_token (p, COMMA)) {
	    next_token (p);
	}
    }
    
    expect_token (p, RPAREN);
    return true;
}

NodePtr
parse_function (Parser *p) {
    Location start_locus = peek_token (p).locus;
    NodePtr node = make_node (p->allocator, NODE_Func, start_locus);
    FunctionItem *func = &node->function;
    expect_token (p, FUNC);
    if (!parse_identifier (p, &func->name)) todo ();
    if (!parse_function_parameters (p, &func->parameters)) todo ();

    func->return_type = parse_type (p);
    todo ();
}
