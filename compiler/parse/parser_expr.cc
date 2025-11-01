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
    typedef ast::NodePtr(*ParsePostfix)(parse::Parser *, ast::NodePtr);
    typedef ast::NodePtr(*ParsePrefix)(parse::Parser *);

    struct ParseRule {
	ParsePrefix prefix = nullptr;
	ParseInfix infix = nullptr;
	ParsePostfix postfix = nullptr;
	Precedence   precedence = Precedence::None;
    };

    ast::NodePtr parse_block (Parser *p);
    ast::NodePtr parse_primary (Parser *p);
    ast::NodePtr parse_unary (Parser *p);
    ast::NodePtr parse_binary  (Parser *p, ast::NodePtr left);

#define RULE(pr, in, po, prec) ParseRule {pr, in, po, prec}

    ParseRule parse_rules[] = {
	[lex::IDENTIFIER]       = RULE (parse_primary, nullptr, nullptr, Precedence::Primary),
	[lex::INT_LITERAL]      = RULE (parse_primary, nullptr, nullptr, Precedence::Primary),
	[lex::STRING_LITERAL]   = RULE (parse_primary, nullptr, nullptr, Precedence::Primary),
	[lex::AMP]              = RULE (parse_unary, parse_binary, nullptr, Precedence::BitAnd),
	[lex::OR]               = RULE (parse_unary, parse_binary, nullptr, Precedence::BitOr),
	[lex::PLUS]             = RULE (nullptr, parse_binary, nullptr, Precedence::Additive),
	[lex::MINUS]            = RULE (nullptr, parse_binary, nullptr, Precedence::Additive),
	[lex::ASTERISK]         = RULE (nullptr, parse_binary, nullptr, Precedence::Multiplicative),
	[lex::DIV]              = RULE (nullptr, parse_binary, nullptr, Precedence::Multiplicative),
	[lex::LBRACE]           = RULE (parse_block, nullptr, nullptr, Precedence::Primary),
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
	    auto rule = get_rule (tok);
	    if (op == lex::END_OF_FILE) break;
        
	    if ((int)rule.precedence < min_prec) break;

	    if (rule.infix) left = rule.infix (p, std::move (left));
	    else if (rule.postfix) left = rule.postfix (p, std::move (left));
	    else break;
	}
	return left;
    }

    ast::NodePtr
    parse_primary (Parser *p) {
	todo ();
    }

    ast::NodePtr 
    parse_binary (Parser *p, ast::NodePtr left) {
	todo ();
    }

    ast::NodePtr parse_unary(Parser *p) { todo(); }

    ast::NodePtr
    parse_block (Parser *p) {
	ast::Block block_node;
	expect_token (p, lex::LBRACE);
	while (!at_end_or_is (p, lex::RBRACE)) {
	    ast::NodePtr node = parse_expression (p);
	    if (node) {
		block_node.add_node (std::move(node));
	    }
	}
	expect_token (p, lex::RBRACE);
	return std::make_unique<ast::Block>(block_node);
    }

} // tuff::parser
