#pragma once

#include "ast/ast.hh"
#include "lex/lexer.hh"
#include "compiler.hh"
#include "unit/module.hh"

namespace tuff::parse {
    struct Parser {
    public:
	lex::Lexer lexer;
	ModuleId   module_id;
	CompileSession *sess;
	Allocator *allocator;
	Parser (ModuleId id, CompileSession *sess);
	void parse ();
	lex::TokenPtr peek_token ();
	lex::TokenPtr next_token ();
	void add_item (ast::NodePtr item);
	
    private:
	lex::TokenPtr current_token;
	unit::Module *mod;
    };
} // tuff::parse
