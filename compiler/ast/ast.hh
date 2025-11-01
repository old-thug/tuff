#pragma once

#include "array.hh"
#include "ast/type.hh"

#include "lex/loc.hh"
#include "sv.hh"
#include <utility>

namespace tuff::ast {
    struct Node;
    using NodePtr = Node *;

    enum struct NodeKind {
	ItemBegin,
	
	Function,
	UseDecl,
	
	ItemEnd,
	ExprBegin,
	
	Identifier,
	Integer,
	Block,
	Boolean,
	String,
	Character,
	Declare,
	
	ExprEnd,
    };

    struct Node {
	virtual ~Node () = default;
	virtual lex::Location loc () const = 0;
	virtual NodeKind kind () const = 0;
    };

    struct Identifier: Node {
	StringView value;
	lex::Location    location;
	
	lex::Location loc () const override
	{
	    return this->location;
	}
	
	NodeKind kind () const override
	{
	    return NodeKind::Identifier;
	}

	Identifier (lex::Location loc, StringView val)
	    : value (val), location (loc) {}

	Identifier () = default;
    };

    struct Block : Node {
	lex::Location begin;
	lex::Location end;
	Array<NodePtr> items;

	lex::Location
	loc () const override {
	    return this->begin.merge(this->end);
	}
	
	NodeKind
	kind () const override {
	    return NodeKind::Block;
	}

	Block ()
	    : items() {}

	void
	add_node (NodePtr node) {
	    this->items.push(node);
	}
    };

    struct Declare: Node {
	lex::Location _loc;
	NodePtr       reciever;
	TypePtr       type;
	NodePtr       expr;

	NodeKind
	kind () const override {
	    return NodeKind::Declare;
	}

	lex::Location
	loc () const override {
	    return this->_loc;
	}
    };
    
    struct Function : Node {
	struct Parameter {
	    Identifier param_name;
	    TypePtr    type;

	    Parameter (Identifier name, TypePtr type)
		: param_name(name), type (type) {}

	    Parameter () = default;
	};
	using ParameterList = Array<Function::Parameter>;
	
	lex::Location begin;
	lex::Location end;

	Identifier       function_name;
	Function::ParameterList parameters;
	TypePtr          return_type;
	NodePtr          body;

	lex::Location
	loc () const override {
	    return this->begin.merge(this->end);
	}
	
	NodeKind
	kind () const override {
	    return NodeKind::Function;
	}

	Function (lex::Location loc,
		  Identifier fname,
		  Function::ParameterList params,
		  TypePtr return_type, NodePtr body)
	    : begin (loc), function_name (fname),
	      parameters (params), return_type (return_type),
	      body (body) {}
    };
} // tuff::ast
