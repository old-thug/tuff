#include "parse/parser.hh"
#include "lex/token.hh"
#include "ast/ast.hh"
#include "def.hh"
#include <memory>

namespace tuff::parse {
    extern bool expect_token (Parser *p, lex::TokenType type);
    extern bool match_token (Parser *p, lex::TokenType type);
    extern bool at_end_or_is (Parser *p, lex::TokenType type);

    enum struct Precedence {
	None,              // no precedence
	Comma,             // ,
	Assign,            // =, +=, -=, etc.
	Ternary,           // ?:
	LogicalOr,         // ||
	LogicalAnd,        // &&
	BitOr,             // |
	BitXor,            // ^
	BitAnd,            // &
	Equality,          // ==, !=
	Relational,        // <, <=, >, >=
	Shift,             // <<, >>
	Additive,          // +, -
	Multiplicative,    // *, /, %
	Unary,             // !, ~, +, -, *
	Postfix,           // ++, --, (), [], .
	Primary            // literals, identifiers
    };

    typedef ast::NodePtr(*ParseInfix)(parse::Parser *, ast::NodePtr);
    typedef ast::NodePtr(*ParsePrefix)(parse::Parser *);

    struct ParseRule {
	ParsePrefix prefix = nullptr;
	ParseInfix infix = nullptr;
	Precedence   precedence = Precedence::None;
    };

    ast::NodePtr parse_block (Parser *p);
    ast::NodePtr parse_primary (Parser *p);
    ast::NodePtr parse_unary (Parser *p);
    ast::NodePtr parse_binary  (Parser *p, ast::NodePtr left);
    ast::NodePtr parse_assign (Parser *p, ast::NodePtr left);
    
#define RULE(pr, in, prec) ParseRule {pr, in, prec}

    ParseRule parse_rules[] = {
	[lex::ASSIGN]           = RULE (nullptr, parse_assign, Precedence::Assign),
	[lex::IDENTIFIER]       = RULE (parse_primary, nullptr, Precedence::Primary),
	[lex::INT_LITERAL]      = RULE (parse_primary, nullptr, Precedence::Primary),
	[lex::STRING_LITERAL]   = RULE (parse_primary, nullptr, Precedence::Primary),
	[lex::AMP]              = RULE (parse_unary, parse_binary, Precedence::BitAnd),
	[lex::OR]               = RULE (parse_unary, parse_binary, Precedence::BitOr),
	[lex::PLUS]             = RULE (nullptr, parse_binary, Precedence::Additive),
	[lex::MINUS]            = RULE (nullptr, parse_binary, Precedence::Additive),
	[lex::ASTERISK]         = RULE (nullptr, parse_binary, Precedence::Multiplicative),
	[lex::DIV]              = RULE (nullptr, parse_binary, Precedence::Multiplicative),
	[lex::LBRACE]           = RULE (parse_block, nullptr, Precedence::Primary),
    };

    const ParseRule
    get_rule (lex::TokenType type) {
	return parse_rules[type];
    }

    ast::NodePtr
    parse_expression(Parser *p, int min_prec = 0) {    
	auto tok = p->peek_token()->type;
	auto rule = get_rule (tok);
    
	if (!rule.prefix) todo ("expected start of primary expression: %s", lex::token_str(tok));
	auto left = rule.prefix (p);

	for (;;) {
	    auto op = p->peek_token ()->type;
	    auto rule = get_rule (op);
	    if (op == lex::END_OF_FILE) break;
	    if ((int)rule.precedence < min_prec) break;

	    if (rule.infix) left = rule.infix (p, left);
	    else break;
	}
	return left;
    }

    ast::NodePtr
    parse_primary (Parser *p) {
	auto tok = p->next_token ();
	switch (tok->type) {
	case lex::IDENTIFIER:
	    return p->allocator->make<ast::Identifier>(tok->loc, tok->str);
	    break;
	default: todo ();
	}
    }

    ast::NodePtr 
    parse_binary (Parser *p, ast::NodePtr left) {
	todo ();
    }

    ast::NodePtr parse_unary(Parser *p) { todo(); }

    ast::NodePtr
    parse_block (Parser *p) {
	ast::Block block_node;
	p->next_token ();
	while (!match_token (p, lex::RBRACE)) {
	    ast::NodePtr node = parse_expression (p);
	    if (node) {
		block_node.add_node (node);
	    }
	}
	expect_token (p, lex::RBRACE);
	return p->allocator->make<ast::Block>(block_node);
    }

    ast::NodePtr
    parse_assign (Parser *p, ast::NodePtr left) {
	todo ();
    }
} // tuff::parser
