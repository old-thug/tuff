#pragma once

#include <memory>

#include "lex/loc.hh"
#include "sv.hh"

namespace tuff::lex {
#define TUFF_TOKEN_LIST				\
    TUFF_TOKEN (END_OF_FILE, "end of file")				\
    TUFF_TOKEN (FUNC, "func")						\
    TUFF_TOKEN (USE, "use")						\
    TUFF_TOKEN (EXTERN, "extern")					\
    TUFF_TOKEN (VOID, "void")						\
    TUFF_TOKEN (I8, "i8")						\
    TUFF_TOKEN (I16, "i16")						\
    TUFF_TOKEN (I32, "i32")						\
    TUFF_TOKEN (I64, "i64")						\
    TUFF_TOKEN (U8, "u8")						\
    TUFF_TOKEN (U16, "u16")						\
    TUFF_TOKEN (U32, "u32")						\
    TUFF_TOKEN (U64, "u64")						\
									\
    TUFF_TOKEN (LBRACE, "{")						\
    TUFF_TOKEN (RBRACE, "}")						\
    TUFF_TOKEN (LPAREN, "(")						\
    TUFF_TOKEN (RPAREN, ")")						\
    TUFF_TOKEN (LBRACK, "[")						\
    TUFF_TOKEN (RBRACK, "]")						\
    TUFF_TOKEN (EXCLAM, "!")						\
    TUFF_TOKEN (NOT_EQUAL, "!=")					\
    TUFF_TOKEN (PERCENT, "%")						\
    TUFF_TOKEN (PERCENT_EQ, "%=")					\
    TUFF_TOKEN (AMP, "&")						\
    TUFF_TOKEN (AMP_EQ, "&=")						\
    TUFF_TOKEN (LOGICAL_AND, "&&")					\
    TUFF_TOKEN (PLUS, "+")						\
    TUFF_TOKEN (MINUS, "-")						\
    TUFF_TOKEN (DIV, "/")						\
    TUFF_TOKEN (ASTERISK, "*")						\
    TUFF_TOKEN (PLUS_EQ, "+=")						\
    TUFF_TOKEN (MINUS_EQ, "-=")						\
    TUFF_TOKEN (ASTERISK_EQ, "*=")					\
    TUFF_TOKEN (COMMA, ",")						\
    TUFF_TOKEN (LIGHT_ARROW, "->")					\
    TUFF_TOKEN (DOT, ".")						\
    TUFF_TOKEN (DOT_DOT, "..")						\
    TUFF_TOKEN (DOT_DOT_EQ, "..=")					\
    TUFF_TOKEN (ELLIPSIS, "...")					\
    TUFF_TOKEN (DIV_EQ, "/=")						\
    TUFF_TOKEN (COLON, ":")						\
    TUFF_TOKEN (ASSIGN, ":=")						\
    TUFF_TOKEN (SEMICOLON, ";")						\
    TUFF_TOKEN (LEFT_SHIFT, "<<")					\
    TUFF_TOKEN (LEFT_SHIFT_EQ, "<<=")					\
    TUFF_TOKEN (LEFT_ANGLE, "<")					\
    TUFF_TOKEN (LESS_OR_EQUAL, "<=")					\
    TUFF_TOKEN (EQUAL, "=")						\
    TUFF_TOKEN (EQUAL_EQUAL, "==")					\
    TUFF_TOKEN (MATCH_ARROW, "=>")					\
    TUFF_TOKEN (RIGHT_ANGLE, ">")					\
    TUFF_TOKEN (GREATER_OR_EQUAL, ">=")					\
    TUFF_TOKEN (RIGHT_SHIFT, ">>")					\
    TUFF_TOKEN (RIGHT_SHIFT_EQ, ">>=")					\
    TUFF_TOKEN (INTRISIC, "@")					\
    TUFF_TOKEN (TILDE, "~")						\
    TUFF_TOKEN (BACKSLASH, "\\")					\
    TUFF_TOKEN (BACKTICK, "`")						\
    TUFF_TOKEN (CARET, "^")						\
    TUFF_TOKEN (CARET_EQ, "^=")						\
    TUFF_TOKEN (PIPE, "|")						\
    TUFF_TOKEN (PIPE_EQ, "|=")						\
    TUFF_TOKEN (OR, "||")						\
    TUFF_TOKEN (CONST_ASSIGN, "::")					\
    TUFF_TOKEN (IDENTIFIER, "identifier")				\
    TUFF_TOKEN (INT_LITERAL,						\
		"integer literal")					\
    TUFF_TOKEN (FLOAT_LITERAL, "float literal")				\
    TUFF_TOKEN (STRING_LITERAL, "string literal")			\
    TUFF_TOKEN (CHAR_LITERAL, "character literal")			

    enum TokenType {
#define TUFF_TOKEN(name, s) name,
	TUFF_TOKEN_LIST
#undef TUFF_TOKEN
    };
    
    struct Token {
	TokenType type;
	Location  loc;
	StringView str;

	Token (TokenType type, Location loc)
	    : type(type), loc(loc) {}

	Token (TokenType type, Location loc, StringView str)
	    : type(type), loc(loc), str(str) {}
    };

    typedef std::shared_ptr<Token> TokenPtr;

    inline TokenPtr make_token (TokenType type, Location loc)
    {
	return std::make_shared<Token>(type, loc);
    }

    inline TokenPtr make_token (TokenType type, Location loc, StringView str)
    {
	return std::make_shared<Token>(type, loc, str);
    }

    inline const char *token_str (TokenType type)
    {
	return (const char * []) {
#define TUFF_TOKEN(name, s) s,
	    TUFF_TOKEN_LIST
#undef TUFF_TOKEN
	} [type];
    }
} // tuff::lex
