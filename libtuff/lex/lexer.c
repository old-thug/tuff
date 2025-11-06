#include "lex/lexer.h"
#include "lex/token.h"
#include "sv.h"
#include <stdbool.h>
#include <stdint.h>

#define save_loc(l) \
    l->prev_line = l->line;\
    l->prev_col  = l->col

#define get_loc(l) \
    make_locus (l->prev_line, l->line,\
		l->prev_col, l->col,\
		l->module_id)

static struct {
    const char *word;
    int id;
} KEYWORDS[] = {
    { "func", FUNC },
    { "extern", EXTERN },
    { "use", USE },
    { "void", VOID },
    { "var", VAR },
    { "const", CONST },
    { "if", IF },
    { "else", ELSE },
    { "i8", I8 },
    { "i16", I16 },
    { "i32", I32 },
    { "i64", I64 },
    { "u8", U8 },
    { "u16", U16 },
    { "u32", U32 },
    { "u64", U64 },
};
static int KEYWORD_LEN = sizeof (KEYWORDS)/sizeof (KEYWORDS[0]);

NOTE (old-thug,
      "the order of this listing matters",
      "punctuations like `==` need to appear before `=` so it doesn't lex as `=` `=` but as `==`");
static
struct {
    const char *word;
    int id;
} PUNCTS[] = {
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
    { ":=", COLEQ },
    { "::", CONST_ASSIGN },
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
static int PUNCTS_LEN = sizeof (PUNCTS)/sizeof (PUNCTS[0]);
typedef uint32_t Codepoint;

int
utf8_decode (const char *s, Codepoint *cp) {
    unsigned char c = (unsigned char)s[0];
    if (c <= 0x7F) {
        *cp = c;
        return 1;
    } else if ((c & 0xE0) == 0xC0) {
        *cp = ((c & 0x1F) << 6) | (s[1] & 0x3F);
        return 2;
    } else if ((c & 0xF0) == 0xE0) {
        *cp = ((c & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        return 3;
    } else if ((c & 0xF8) == 0xF0) {
        *cp = ((c & 0x07) << 18) | ((s[1] & 0x3F) << 12) |
	    ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        return 4;
    }
    return -1; // invalid UTF-8
}

bool
is_alpha (Codepoint cp) {
    return (cp == '_') || (cp >= 'a' && cp <= 'z')
	|| (cp >= 'A' && cp <= 'Z') || (cp > 127); 
}

bool
is_digit (Codepoint cp) {
    return (cp >= '0' && cp <= '9');
}

// Get the next codepoint from the lexer.
static Codepoint
lexer_next_codepoint (Lexer *l) {
    if (*(l->cursor) == 0) return 0;
    Codepoint cp;
    int len = utf8_decode (l->cursor, &cp);
    if (len < 0) {
	l->cursor ++;
	return 0xFFFD;
    }

    l->cursor += len;
    l->col ++;
    return cp;
}

// Get the next codepoint with out advancing the lexer.
static Codepoint
lexer_peek_codepoint(Lexer *l) {
    if (*(l->cursor) == '\0') return 0; // EOF
    Codepoint cp;
    int len = utf8_decode(l->cursor, &cp);
    if (len < 0) return 0xFFFD;
    return cp;
}

static StringView
lexer_slice (Lexer *l, const char *start) {
    return sv_init (start, l->cursor - start);
}

Token
lexer_get_token (Lexer *l) {
    // Skip whitespace.
    while (1) {
	Codepoint cp = lexer_peek_codepoint (l);
	if (cp == 0) break;
	if (cp == ' ' || cp == '\t') {
	    lexer_next_codepoint (l);
	    continue;
	}

	if (cp == '\n') {
	    lexer_next_codepoint (l);
	    l->line ++;
	    l->col  = 1;
	    continue;
	} else if (cp == '\r' && *(l->cursor + 1) == '\n') {
	    lexer_next_codepoint (l);
	    lexer_next_codepoint (l);
	    l->line ++;
	    l->col  = 1;
	    continue;
	} else if (cp == '/' && *(l->cursor + 1) == '/') {
	    // Single-line comment.
	    // Skip until end of line or EOF.
	    while (cp != '\n' && cp != '\r' && cp != 0) {
		cp = lexer_next_codepoint (l);
	    }
	    // Consume the newline so the next token starts clean.
	    Codepoint pcp = cp;
	    if (cp != 0) 
		if (cp == '\r') {
		    cp = lexer_next_codepoint (l);
		    if (cp == '\n')
			cp = lexer_next_codepoint (l);
		}
	    continue;
	} else if (cp == '/' && *(l->cursor + 1) == '*') {
	    lexer_next_codepoint (l); // consume '*'
	    cp = lexer_next_codepoint (l);
	    int depth = 1;

	    while (cp != 0 && depth > 0) {
		if (cp == '/' && *(l->cursor + 1) == '*') {
		    depth++;
		    lexer_next_codepoint (l); // consume '*'
		} else if (cp == '*' && *(l->cursor + 1) == '/') {
		    depth--;
		    lexer_next_codepoint (l); // consume '/'
		}
		cp = lexer_next_codepoint (l);
	    }

	    if (depth > 0) {
		todo ("error: unterminated block comment");
	    }

	    cp = lexer_next_codepoint (l);
	    continue;
	}
	
	break;
    }

    save_loc (l);
    const char *start = l->cursor;
    Codepoint cp = lexer_peek_codepoint (l);

    if (cp == 0) {
	return make_token_x (END_OF_FILE, get_loc (l));
    }

    if (is_alpha (cp)) {
	while (1) {
	    Codepoint next;
	    int len = utf8_decode(l->cursor, &next);
	    if (len < 0) break;
	    if (!is_alpha(next) && !is_digit(next)) break;
	    l->cursor += len;
	    l->col    += len;
	}
	StringView word = lexer_slice (l, start);

	for (int n = 0; n < KEYWORD_LEN; n ++) {
	    StringView query = sv_init (KEYWORDS[n].word, strlen (KEYWORDS[n].word));
	    if (sv_eq (word, query)) {
		return make_token_x (KEYWORDS[n].id, get_loc (l));
	    }
	}
	return make_token (IDENTIFIER, get_loc (l), word);
    }

    if (is_digit(cp)) {
	const char *start = l->cursor;
	Codepoint next = cp;
	int base = 10;
	
	// check for 0x, 0b, 0o prefixes
	if (next == '0') {
	    const char *peek = l->cursor + 1;
	    if (*peek == 'x' || *peek == 'X') { base = 16; l->cursor += 2; }
	    else if (*peek == 'b' || *peek == 'B') { base = 2; l->cursor += 2; }
	    else if (*peek == 'o' || *peek == 'O') { base = 8; l->cursor += 2; }
	}
	
	// main digit loop
	while (1) {
	    int len = utf8_decode(l->cursor, &next);
	    if (len < 0) break;
	    bool valid = false;
	    
	    switch (base) {
            case 2:  valid = (next == '0' || next == '1'); break;
            case 8:  valid = (next >= '0' && next <= '7'); break;
            case 10: valid = is_digit(next); break;
            case 16: valid = is_digit(next) ||
		    (next >= 'a' && next <= 'f') ||
		    (next >= 'A' && next <= 'F'); break;
	    }

	    // Allow for 1_000_000 style digits;
	    if (next == '_') { l->cursor += len; continue; }
	    
	    if (!valid) break;
	    l->cursor += len;
	    l->col += len;
	}

	StringView digits = lexer_slice(l, start);
	return make_token(INT_LITERAL, get_loc(l), digits);
    }

    for (int n = 0; n < PUNCTS_LEN; n++) {
        StringView query = sv_init (PUNCTS[n].word, strlen (PUNCTS[n].word));
        size_t query_len = query.len;
        // slice the same number of bytes from the source
        StringView slice = sv_init (start, query_len);
        if (sv_eq (slice, query)) {
             // if multi-char, advance cursor accordingly
            l->cursor += query_len;
	    l->col += query_len;
            return make_token_x(PUNCTS[n].id, get_loc(l));
        }
    }

    todo ("LEX_FAILED: `%c` `%s`", *start, start);
}
