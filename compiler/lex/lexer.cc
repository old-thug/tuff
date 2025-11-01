#include "lex/lexer.hh"

#include <cctype>

#include "def.hh"
#include "lex/token.hh"

static bool
is_alpha(tuff::lex::Lexer::Codepoint cp)
{
    return (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z') || (cp >= 0x80);
}

static bool
is_digit(tuff::lex::Lexer::Codepoint cp) {
    return (cp >= '0' && cp <= '9');
}

namespace tuff::lex {
    static const
    struct { const char *word; TokenType tok; }
	KEYWORDS[] = {
	{ "func", FUNC },
	{ "use", USE },
	{ "extern", EXTERN },
	{ "void", VOID },
	{ "i8", I8 },
	{ "i16", I16 },
	{ "i32", I32 },
	{ "i64", I64 },
	{ "u8", U8 },
	{ "u16", U16 },
	{ "u32", U32 },
	{ "u64", U64 },
    };

    static const
    struct { const char *word; TokenType tok; } PUNCTS[] = {
	{ "{", LBRACE },
	{ "}", RBRACE },
	{ "(", LPAREN },
	{ ")", RPAREN },
	{ "[", LBRACK },
	{ "]", RBRACK },
	{ "!=", NOT_EQUAL },
	{ "!", EXCLAM },
	{ "%=", PERCENT_EQ },
	{ "%", PERCENT },
	{ "&=", AMP_EQ },
	{ "&&", LOGICAL_AND },
	{ "&", AMP },
	{ "+=", PLUS_EQ },
	{ "+", PLUS },
	{ "-=", MINUS_EQ },
	{ "->", LIGHT_ARROW },
	{ "-", MINUS },
	{ "*=", ASTERISK_EQ },
	{ "*", ASTERISK },
	{ ",", COMMA },
	{ "...", ELLIPSIS },
	{ "..=", DOT_DOT_EQ },
	{ "..", DOT_DOT },
	{ ".", DOT },
	{ "/=", DIV_EQ },
	{ "/", DIV },
	{ "::", CONST_ASSIGN },
	{ ":=", ASSIGN },
	{ ":", COLON },
	{ ";", SEMICOLON },
	{ "<<=", LEFT_SHIFT_EQ },
	{ "<<", LEFT_SHIFT },
	{ "<=", LESS_OR_EQUAL },
	{ "<", LEFT_ANGLE },
	{ "==", EQUAL_EQUAL },
	{ "=>", MATCH_ARROW },
	{ "=", EQUAL },
	{ ">>=", RIGHT_SHIFT_EQ },
	{ ">>", RIGHT_SHIFT },
	{ ">=", GREATER_OR_EQUAL },
	{ ">", RIGHT_ANGLE },
	{ "@", INTRISIC },
	{ "~", TILDE },
	{ "\\", BACKSLASH },
	{ "`", BACKTICK },
	{ "^=", CARET_EQ },
	{ "^", CARET },
	{ "||", OR },
	{ "|=", PIPE_EQ },
	{ "|", PIPE },
    };

    
    // std::string encode_utf8(Lexer::Codepoint cp) {
    // 	std::string out;
    // 	if (cp < 0x80) {
    // 	    out += static_cast<char>(cp);
    // 	} else if (cp < 0x800) {
    // 	    out += static_cast<char>(0xC0 | (cp >> 6));
    // 	    out += static_cast<char>(0x80 | (cp & 0x3F));
    // 	} else if (cp < 0x10000) {
    // 	    out += static_cast<char>(0xE0 | (cp >> 12));
    // 	    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    // 	    out += static_cast<char>(0x80 | (cp & 0x3F));
    // 	} else {
    // 	    out += static_cast<char>(0xF0 | (cp >> 18));
    // 	    out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
    // 	    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    // 	    out += static_cast<char>(0x80 | (cp & 0x3F));
    // 	}
    // 	return out;
    // }
    
    Lexer::Lexer (ModuleId id, CompileSession *session)
	: module_id(id), source(session->get_module (id)->source())
    {
    }

    void
    Lexer::save_loc ()
    {
	this->cursor.pline = this->cursor.line;
	this->cursor.pcol = this->cursor.col;
    }

    Location
    Lexer::get_loc ()
    {
	return Location (this->cursor.pline,
			 this->cursor.line,
			 this->cursor.pcol,
			 this->cursor.col-1,
			 this->module_id);
    }
    
    TokenPtr
    Lexer::next_token ()
    {
	auto cp = peek_codepoint ();
	if (isspace (cp)) {
	    while (isspace (cp)) {
		eat ();
		cp = peek_codepoint ();
	    }
	}

	save_loc ();
	
	if (cp == 0 || this->position >= strlen (this->source)) {
	    return make_token (END_OF_FILE, get_loc ());
	}

	if (is_alpha (cp) || cp == '_') {
	    size_t start = this->position;
	    while (is_alpha (cp) || is_digit (cp) || cp == '_') {
		eat ();
		cp = peek_codepoint ();
	    }
	    size_t end   = this->position;
	    auto word = slice (start, end);
	    for (const auto& pair: KEYWORDS) {
		if (word == pair.word) {
		    return make_token (pair.tok, get_loc ());
		}
	    }
	    return make_token (IDENTIFIER, get_loc (), word);
	}

	for (const auto& pair: PUNCTS) {
	    if (slice (this->position, strlen (pair.word) + this->position) == pair.word) {
		eat (strlen (pair.word));
		return make_token (pair.tok, get_loc ());
	    }
	}
	
	todo ();
	return make_token (END_OF_FILE, get_loc ());
    }

    Lexer::Codepoint
    Lexer::peek_codepoint() const {
	size_t pos = this->position;
	uint8_t c = this->source[pos++];
	if (c < 0x80) return c;

	Lexer::Codepoint cp = 0;
	int bytes = 0;

	if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; bytes = 1; }
	else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; bytes = 2; }
	else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; bytes = 3; }
	else return 0xFFFD;

	while (bytes--) {
	    if (pos >= strlen (this->source)) return 0xFFFD;
	    uint8_t t = this->source[pos++];
	    if ((t & 0xC0) != 0x80) return 0xFFFD;
	    cp = (cp << 6) | (t & 0x3F);
	}

	return cp;
    }

    
    Lexer::Codepoint
    Lexer::next_codepoint ()
    {
	uint8_t c = this->source[this->position++];
	if (c < 0x80) return c; // ASCII.
	
	Lexer::Codepoint cp = 0;
	int bytes = 0;
	
	if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; bytes = 1; }
	else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; bytes = 2; }
	else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; bytes = 3; }
	else return 0xFFFD; // Invalid.

	while (bytes--) {
	    if (this->position >= strlen (this->source)) return 0xFFFD;
	    uint8_t t = this->source[this->position++];
	    if ((t & 0xC0) != 0x80) return 0xFFFD;
	    cp = (cp << 6) | (t & 0x3F);
	}

	return cp;
    }
    
    Lexer::Codepoint
    Lexer::eat ()
    {
	auto cp = peek_codepoint ();
	if (cp == '\n') {
	    this->cursor.line ++;
	    this->cursor.col  = 1;
	} else {
	    this->cursor.col ++;
	}
	this->position ++;
	return cp;
    }

    Lexer::Codepoint
    Lexer::eat (size_t n)
    {
	Codepoint cp = 0;
	for (int i = 0; i < n; i++) {
	    eat();
	}
	return cp;
    }

    StringView
    Lexer::slice (size_t start, size_t end)
    {
	return StringView(this->source).substr(start, end - start);
    }
} // tuff::lex
