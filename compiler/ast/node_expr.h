#ifndef _TUFF_NODE_EXPR_H_
#define _TUFF_NODE_EXPR_H_

#include "array.h"
#include "ast/node_kind.h"
#include "compiler.h"
#include "lex/loc.h"
#include "sv.h"

typedef struct {
    StringView value;
    Location   locus;
} IdentifierExpr;

typedef struct {
    arr_of (NodePtr) expressions;
    Location begin;
    Location end;
} BlockExpr;

typedef struct {
    NodePtr value;
} ReturnExpr;

NodePtr
make_block_expr (Allocator *allocator, Location begin_locus);
void
set_block_end_locus (NodePtr block, Location locus);
void
append_node_to_block (NodePtr block, NodePtr node);

NodePtr
make_return_expr (Allocator *alloctor, NodePtr value, Location locus);

#endif // _TUFF_NODE_EXPR_H_
