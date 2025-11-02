#ifndef _TUFF_NODE_EXPR_H_
#define _TUFF_NODE_EXPR_H_

#include "lex/loc.h"
#include "sv.h"

typedef struct {
    StringView value;
    Location   locus;
} IdentifierExpr;


#endif // _TUFF_NODE_EXPR_H_
