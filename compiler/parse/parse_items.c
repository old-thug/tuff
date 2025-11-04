#include "ast/node.h"
#include "ast/node_expr.h"
#include "ast/node_item.h"
#include "ast/node_kind.h"
#include "diag/diag.h"
#include "lex/loc.h"
#include "lex/token.h"
#include "parse/parser.h"
#include "sv.h"

extern NodePtr
parse_expr (Parser *p, int min_prec);
extern NodePtr
parse_block (Parser *p);

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
	default: return false;
	}
    } 
    return false;
}

bool
parse_identifier (Parser *p, IdentifierExpr *ident) {
    if (!match_token (p, IDENTIFIER)) {
	parse_error (p, DIAG_Error, p->current_token.locus, NULL,
		     "expected an identifer here, got `%s`",
		     token_name (p->current_token.id));
	next_token (p);
	return false;
    }
    Token tok = next_token (p);
    ident->value = tok.str;
    ident->locus = tok.locus;
    return true;
}

bool
parse_function_parameters (Parser *p, FunctionParameterList *params) {
    if (!match_token (p, LPAREN)) {
	parse_error (p, DIAG_Error,
		     p->current_token.locus, "expected `(` here",
		     "expected start of function parameter list");
	skip_to_after (p, RPAREN);
	return false;
    }
    next_token (p);
    bool seen_default_value = false, reported = false;
    IdentifierExpr first_param_default_value_set;
    while (!is_at_end_or_is (p, RPAREN)) {
	FunctionParameter param;
	if (!parse_identifier (p, &param.name)) {
	    skip_to (p, RPAREN);
	    break;
	}

	expect_token (p, COLON);
	param.type = parse_type (p);

	if (match_token (p, EQUAL)) {
	    next_token (p);
	    param.default_value = parse_expr (p, 0);
	    if (!seen_default_value) {
		seen_default_value = true;
		first_param_default_value_set = param.name;
	    }
	} else {
	    if (seen_default_value && !reported) {
		Diagnostic *diag = parse_error (p, DIAG_Error, param.name.locus, "move this before any defaulted parameters",
						"cannot declare a non-default parameter (`%.*s`) after a parameter with a default value",
						LIT (param.name.value));
		diag_simple_note (diag, "parameters with default values must appear after all required parameters"); 
		diag_note (diag, first_param_default_value_set.locus, "here",
			   "parameter `%.*s` first introduced a default value",
			   LIT (first_param_default_value_set.value));
		reported = true;
	    }
	    
	    TODO (old-thug,
		 "enhance diagnostics: suggest a 'fixed' order for parameters",
		 "Example: for `func foo(a: i32 = f, b: i32, c: i32)`",
		 "suggest reordering to `func foo(b: i32, c: i32, a: i32 = f)`");
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
    if (!parse_identifier (p, &func->name)) {
	skip_to (p, LPAREN);
    }
    
    if (!parse_function_parameters (p, &func->parameters)) {
	skip_to_next_type (p);
    }

    func->return_type = parse_type (p);
    if (match_token (p, LBRACE)) {
	func->expression = parse_block (p);
    } else {
	func->expression = parse_expr (p, 0);
    }
    return node;
}
