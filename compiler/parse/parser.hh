#pragma once

#include "lex/lexer.hh"
#include "compiler.hh"

namespace tuff::parse {
    struct Parser {
    public:
	lex::Lexer lexer;
	ModuleId   module_id;
	CompileSession *sess;
	Parser (ModuleId id, CompileSession *sess);
	void parse ();
	lex::TokenPtr peek_token ();
	lex::TokenPtr next_token ();
	
    private:
	lex::TokenPtr current_token;
    };
} // tuff::parse
