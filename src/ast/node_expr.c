#include "ast/node_expr.h"
#include "array.h"
#include "ast/node_kind.h"
#include "compiler.h"
#include "ast/node.h"
#include "lex/loc.h"

#include "def.h"

NodePtr
make_block_expr (Allocator *allocator, Location begin_locus) {
    NodePtr node = make_node (allocator, NODE_Block, begin_locus);
    return node;
}

void
set_block_end_locus (NodePtr block, Location locus) {
    assert (block->kind == NODE_Block, "not a block node");
    block->block.end = locus;
}

void
append_node_to_block (NodePtr block, NodePtr node) {
    assert (block->kind == NODE_Block, "attempt to append to non-block node");
    arr_push (&block->block.expressions, node);
}

NodePtr
make_return_expr (Allocator *alloctor, NodePtr value, Location locus) {
    NodePtr node = make_node (alloctor, NODE_Return, locus);
    node->ret.value = value;
    return node;
}

NodePtr
make_declare_expr (Allocator *allocator, Location locus,
		   Mutability mutability, NodePtr receiver,
		   NodePtr expression, TypePtr type) {
    NodePtr node = make_node (allocator, NODE_Declare, locus);
    node->declare.expression = expression;
    node->declare.mutability = mutability;
    node->declare.receiver   = receiver;
    node->declare.type       = type;
    return node;
}

NodePtr
make_error_node (Allocator *allocator, Location locus) {
    NodePtr node = make_node (allocator, NODE_Error, locus);
    return node;
}

NodePtr
make_assign_expr (Allocator *allocator, Location locus,
		  NodePtr receiver, NodePtr expr) {
    NodePtr node = make_node (allocator, NODE_Assign, locus);
    node->assign.expr = expr;
    node->assign.receiver = receiver;
    return node;
}

NodePtr
make_function_call_expr (Allocator *allocator, Location locus,
			 NodePtr callee, ArgumentList arguments) {
    NodePtr node = make_node (allocator, NODE_Call, locus);
    node->func_call.arguments = arguments;
    node->func_call.callee    = callee;
    return node;
}

NodePtr
make_int_expr (Allocator *allocator, int64_t value, Location locus) {
    NodePtr node = make_node (allocator, NODE_Int, locus);
    node->integer.locus = locus;
    node->integer.value = value;
    return node;
}

NodePtr
make_group_expr (Allocator *allocator, Location begin, Location end,
		 NodeList members) {
    NodePtr node = make_node (allocator, NODE_Group, begin);
    node->group.begin = begin;
    node->group.end   = end;
    node->group.members = members;
    return node;
}

NodePtr
make_if_expr (Allocator *allocator, Location locus, NodePtr condition, NodePtr then_block, NodePtr else_block) {
    NodePtr node = make_node (allocator, NODE_If, locus);
    node->if_expr.then_block = then_block;
    node->if_expr.else_block = else_block;
    node->if_expr.condition = condition;
    return node;
}

NodePtr
make_binop_expr (Allocator *allocator, Location locus, enum BinOp binop,
		 NodePtr left, NodePtr right) {
    NodePtr node = make_node (allocator, NODE_Binary, locus);
    node->binop.left = left;
    node->binop.right = right;
    node->binop.op = binop;
    return node;
}
