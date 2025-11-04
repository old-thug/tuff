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
