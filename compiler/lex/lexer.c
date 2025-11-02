#include "lex/lexer.h"
#include "lex/token.h"

static struct {
    const char *word;
    int id;
} KEYWORDS[] = {
    { "func", FUNC },
    { "extern", EXTERN },
    { "use", USE },
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

NOTE (old-thug, "the order of this listing matters");
static
struct { const char *word; int id; } PUNCTS[] = {
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

Token
lexer_get_token (Lexer *lexer) {

}
