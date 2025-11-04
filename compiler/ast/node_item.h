#ifndef _TUFF_NODE_ITEM_H_
#define _TUFF_NODE_ITEM_H_

#include "ast/node_expr.h"
#include "ast/node_kind.h"
#include "ast/type.h"
#include "array.h"

typedef struct {
    IdentifierExpr name;
    TypePtr    type;
    NodePtr    default_value;
} FunctionParameter;

typedef arr_of (FunctionParameter) FunctionParameterList;

typedef struct {
    IdentifierExpr name;
    FunctionParameterList parameters;
    TypePtr return_type;
    NodePtr expression;
} FunctionItem;

#endif // _TUFF_NODE_ITEM_H_
