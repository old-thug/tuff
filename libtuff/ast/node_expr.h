#ifndef _TUFF_NODE_EXPR_H_
#define _TUFF_NODE_EXPR_H_

#include "array.h"
#include "ast/node_kind.h"
#include "ast/type.h"
#include "lex/loc.h"
#include "sv.h"

#include <stdint.h>
extern void *tuff_alloc (Allocator *, size_t);
typedef arr_of (NodePtr) NodeList;
typedef NodeList ArgumentList;

typedef struct {
    StringView value;
    Location   locus;
} IdentifierExpr;

typedef struct {
    int64_t value;
    Location locus;
} IntegerExpr;

typedef struct {
    NodeList expressions;
    Location begin;
    Location end;
} BlockExpr;

typedef struct {
    Mutability mutability;
    IdentifierExpr ident;
    NodePtr expression;
    TypePtr type;
} DeclareExpr;

typedef struct {
    NodePtr receiver;
    NodePtr expr;
} AssignExpr;

typedef struct {
    NodePtr value;
} ReturnExpr;

typedef struct {
    NodePtr callee;
    ArgumentList arguments;
} FunctionCallExpr;

typedef struct {
    Location begin, end;
    NodeList members;
} GroupExpr;

typedef struct {
    NodePtr condition;
    NodePtr then_block;
    NodePtr else_block;
} IfExpr;

typedef struct {
    enum BinOp {
	BINOP_Add,
	BINOP_Sub,
	BINOP_Mul,
	BINOP_Div,
	BINOP_Equal,
    } op;
    NodePtr left;
    NodePtr right;
} BinaryOpExpr;

NodePtr
make_block_expr (Allocator *allocator, Location begin_locus);
NodePtr
make_declare_expr (Allocator *allocator, Location locus,
		   Mutability mutability, IdentifierExpr ident,
		   NodePtr expression, TypePtr type);
NodePtr
make_assign_expr (Allocator *allocator, Location locus,
		  NodePtr receiver, NodePtr expr);
NodePtr
make_function_call_expr (Allocator *allocator, Location locus,
			 NodePtr callee, ArgumentList arguments);
NodePtr
make_int_expr (Allocator *allocator, int64_t value, Location locus);
NodePtr
make_group_expr (Allocator *allocator, Location begin, Location end,
		 NodeList members);
NodePtr
make_if_expr (Allocator *allocator, Location locus, NodePtr condition, NodePtr then_block, NodePtr else_block);
NodePtr
make_binop_expr (Allocator *allocator, Location locus, enum BinOp binop,
		 NodePtr left, NodePtr right);

void
set_block_end_locus (NodePtr block, Location locus);
void
append_node_to_block (NodePtr block, NodePtr node);

NodePtr
make_return_expr (Allocator *alloctor, NodePtr value, Location locus);

NodePtr
make_error_node (Allocator *allocator, Location locus);

#endif // _TUFF_NODE_EXPR_H_
