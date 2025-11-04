#ifndef _TUFF_NODE_H_
#define _TUFF_NODE_H_

#include "ast/node_item.h"
#include "ast/node_expr.h"
#include "ast/node_kind.h"
#include "compiler.h"
#include "lex/loc.h"


typedef struct Node {
    NodeKind kind;
    Location primary_locus;
    union {
	FunctionItem   function;
	DeclareExpr    declare;
	IdentifierExpr identifier;
	BlockExpr      block;
	ReturnExpr     ret;
    };
} Node;

inlined Node*
make_node (Allocator *allocator, NodeKind kind, Location locus) {
    Node *node = (Node *)tuff_alloc (allocator, sizeof (Node));
    node->primary_locus = locus;
    node->kind = kind;
    return node;
}

#endif // _TUFF_NODE_H_
