#pragma once

#include "lex/token.hh"
#include "compiler.hh"

namespace tuff::lex {
    struct Lexer {
    public:
	Lexer (ModuleId id, CompileSession *session);
	TokenPtr next_token ();
	using Codepoint = uint32_t;
    private:
	ModuleId module_id;
	const char *source;
	
	struct {
	    size_t line = 1;
	    size_t col  = 1;
	    size_t pline = 1;
	    size_t pcol  = 1;
	} cursor;
	size_t position = 0;
	
        Codepoint next_codepoint ();
        Codepoint peek_codepoint () const;
        Codepoint eat ();
        Codepoint eat (size_t n);

	void save_loc ();
	Location get_loc ();

	StringView slice(size_t start, size_t end);
    };
} // tuff::lex
