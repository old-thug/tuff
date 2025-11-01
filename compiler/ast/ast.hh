#pragma once

#include "array.hh"
#include "ast/type.hh"

#include "lex/loc.hh"
#include "sv.hh"
#include <utility>

namespace tuff::ast {
    struct Node;
    using NodePtr = std::unique_ptr<Node>;

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
	    this->items.push(std::move(node));
	}
    };
    
    struct Function : Node {
	struct Parameter {
	    Identifier param_name;
	    TypePtr    type;  
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
	      parameters (std::move (params)), return_type (std::move (return_type)),
	      body (std::move (body)) {}
    };
} // tuff::ast
