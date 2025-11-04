#include "lex/token.h"

const char *
token_name (int id) {
    return (const char *[]) {
#define TUFF_TOKEN(name, str) [name] = str,
	TUFF_TOKEN_LIST
#undef TUFF_TOKEN
    } [id];
}
