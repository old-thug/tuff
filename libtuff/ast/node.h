#ifndef _TUFF_NODE_H_
#define _TUFF_NODE_H_

#include "ast/node_item.h"
#include "ast/node_expr.h"
#include "ast/node_kind.h"
#include "lex/loc.h"

typedef struct Scope Scope;
typedef struct Symbol Symbol;
typedef struct Node {
    NodeKind kind;
    Location primary_locus;
    Scope *scope;
    Symbol *sym;
    union {
	FunctionItem function;
	DeclareExpr declare;
	AssignExpr assign;
	FunctionCallExpr func_call;
	IdentifierExpr identifier;
	IntegerExpr integer;
	BlockExpr block;
	GroupExpr group;
	ReturnExpr ret;
	IfExpr if_expr;
	BinaryOpExpr binop;
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
