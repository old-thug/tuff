#include "parse/parser.hh"

#include "ast/ast.hh"
#include "ast/type.hh"
#include "compiler.hh"
#include "def.hh"
#include "lex/token.hh"
#include <memory>

namespace tuff::parse {
    // Foward Declarations.
    Array<ast::NodePtr> parse_items (Parser *p);
    ast::TypePtr parse_type (Parser *p);
    ast::Identifier parse_ident (Parser *p);
    ast::NodePtr parse_function (Parser *p);
    Array<ast::Function::Parameter> parse_function_param_list (Parser *p);
	ast::NodePtr parse_expression (Parser *p, int min_prec);

#define error(loc, fmt, ...) p->sess->dcollector ()->error_at (loc, fmt, ##__VA_ARGS__)
    
    Parser::Parser (ModuleId id, CompileSession *sess)
	: lexer (id, sess)
    {
	this->module_id = id;
	this->sess	= sess;
	this->current_token = this->lexer.next_token ();
    }

    lex::TokenPtr
    Parser::peek_token () {
	return this->current_token;
    }

    lex::TokenPtr
    Parser::next_token () {
	auto prev = this->current_token;
	this->current_token = this->lexer.next_token ();
	return prev;
    }

    bool
    match_token (Parser *p, lex::TokenType type) {
	return p->peek_token ()->type == type;
    }

    // returns true if current token matches `type`.
    // Adds an error if not, and returns false;
    bool
    expect_token (Parser *p, lex::TokenType type) {
	if (!match_token (p, type)) {
	    p->sess->dcollector ()
		->error_at (p->peek_token ()->loc,
					    "expected `%s`, but got `%s`",
					    lex::token_str (type),
					    lex::token_str (p->peek_token ()->type));
	    p->next_token ();
	    return false;
	}
	
	p->next_token ();
	return true;
    }

    // Returns true if `is` was encountered or `end-of-file` was reached.
    bool
    at_end_or_is (Parser *p, lex::TokenType is) {
	return match_token (p, is) || match_token (p, lex::END_OF_FILE);
    }

    // Skips until `tok` is encountered.
    void
    skip_to_next (Parser *p, lex::TokenType tok) {
	while (!at_end_or_is(p, tok)) p->next_token();
    }

    // Skips until `tok` is encountered, then skips `tok`
    void
    skip_to_next_after (Parser *p, lex::TokenType tok) {
	while (!at_end_or_is(p, tok)) p->next_token ();
	if (match_token (p, tok)) p->next_token ();
    }

    // Skips to the next token capable of beginning an expression.
    void
    skip_to_next_start_of_expression (Parser *p) {
	while (
	       !at_end_or_is (p, lex::IDENTIFIER) &&
	       !at_end_or_is (p, lex::LBRACE)  &&
	       !at_end_or_is (p, lex::LPAREN)) p->next_token ();
    }
    
    void
    Parser::parse () {
	parse_items (this);
    }

    Array<ast::NodePtr>
    parse_items (Parser *p) {
	Array<ast::NodePtr> items = {};
	
	while (p->peek_token ()->type != lex::END_OF_FILE) {
	    switch (p->peek_token ()->type) {
	    case lex::FUNC: {
		auto func = parse_function (p);
	    } break;
	    default: todo ("%s", lex::token_str (p->peek_token ()->type));
	    }
	}	
	return items;
    }

    /// param_list: '(' param... ')'
    /// param: <identifier> ':' <type>
    Array<ast::Function::Parameter>
    parse_function_param_list (Parser *p) {
	Array<ast::Function::Parameter> params;
	if (match_token (p, lex::LPAREN)) {
	    p->next_token ();
	    while (!at_end_or_is (p, lex::RPAREN)) {
		auto param_ident = parse_ident (p);
		if (expect_token (p, lex::COLON)) {
		    auto param_type  = parse_type (p);
		    params.push ({param_ident, std::move (param_type)});
		} else {
		    
		}
		if (match_token (p, lex::COMMA)) {
		    p->next_token ();
		    continue;
		}
	    }
	} else {
	    error (p->peek_token ()->loc,
			    "expected parameter list after function")
		->note (p->peek_token ()->loc,
			"try adding \"()\" here");;
	    skip_to_next (p, lex::RPAREN);
	}
	
	expect_token (p, lex::RPAREN);
	return params;
    }

    /// function: [modifier] 'func' <identifier> param_list <type> <expr>
    ast::NodePtr
    parse_function (Parser *p) {
	TODO (old-thug,
	     "collect function modifiers, e.g: `pub`, `extern`, `inline`, e.t.c as soon as they are supported");
	expect_token (p, lex::FUNC); // skip keyword
	auto fn_loc = p->peek_token ()->loc;
	auto fn_ident = parse_ident (p);
	auto parameters = parse_function_param_list (p);
	ast::TypePtr fn_type    = nullptr;
	if ((fn_type = parse_type (p)) == nullptr) {
	    skip_to_next_start_of_expression (p);
	}
	auto fn_body = parse_expression (p, 0);
	return std::make_unique<ast::Function> (fn_loc, fn_ident,
					       std::move (parameters),
					       std::move (fn_type),
					       std::move (fn_body));
    }

    /// identifier: [a-zA-Z_][a-zA-Z0-9_]*
    ast::Identifier
    parse_ident (Parser *p) {
	auto loc   = p->peek_token ()->loc;
	auto value = p->peek_token ()->str;

	if (match_token (p, lex::IDENTIFIER)) {
	    expect_token (p, lex::IDENTIFIER);
	    return ast::Identifier(loc, value);
	}

        error (loc, "expected an identifier");
	return {loc, value};
    }

    bool
    current_token_can_start_type (Parser *p) {
	switch (p->peek_token ()->type) {
	case lex::IDENTIFIER:
	case lex::I8:
	case lex::I16:
	case lex::I32:
	case lex::I64:
	case lex::U8:
	case lex::U16:
	case lex::U32:
	case lex::U64:
	case lex::VOID:
	case lex::ASTERISK:
	case lex::AMP:
	case lex::LBRACK:
	    return true;
	default: return false;
	}
    }

    ast::IntegerSize
    int_size_from_token (lex::TokenType tok) {
	switch (tok) {
	case lex::I32:
	    return ast::IntegerSize::Int32;
	default: todo ();
	}
    }

    ast::IntegerSign
    int_sign_from_token (lex::TokenType tok) {
	switch (tok) {
	case lex::I32:
	    return ast::IntegerSign::Signed;
	default: todo ();
	}
    }
    
    /// type: <ident> | ['i' | 'u'](8 | 16 | 32 | 64) | 'void'
    ast::TypePtr
    parse_type (Parser *p) {
	lex::TokenPtr tok = p->peek_token ();
	if (!current_token_can_start_type (p)) {
	    error (tok->loc, "expected a type");
	    return nullptr;
	}
	p->next_token ();
	switch (tok->type) {
	case lex::IDENTIFIER:
	case lex::I8:
	case lex::I16:
	case lex::I32:
	case lex::I64:
	case lex::U8:
	case lex::U16:
	case lex::U32:
	case lex::U64:
	    {
		auto sign = int_sign_from_token (tok->type);
		auto size = int_size_from_token (tok->type);
		return make_type(tok->loc, sign, size);
	    } break;
	case lex::VOID:
	    return make_type(ast::TypeKind::Void, tok->loc);
	default: todo ();
	}
    }
} // tuff::parse
