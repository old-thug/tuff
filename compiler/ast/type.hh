#pragma once

#include "lex/loc.hh"

namespace tuff::ast {
    enum struct TypeKind {
	Integer,
	String,
	Boolean,
	Named,
	Void,
    };

    enum struct IntegerSize {
	Int8,
	Int16,
	Int32,
	Int64,
    };

    enum struct IntegerSign {
	Signed,
	Unsigned,
    };

    enum struct Mutability {
	Mutable,
	Const,
    };
    
    struct Type {
	TypeKind kind;
	Mutability mutability;
	lex::Location loc;
	
	union {
	    struct {
		IntegerSign sign;
		IntegerSize size;
	    } int_type;
	} as;

	Type (lex::Location loc, IntegerSign sign, IntegerSize size, Mutability mtb = Mutability::Mutable)
	    : loc (loc) {
	    this->as.int_type.sign = sign;
	    this->as.int_type.size = size;
	    this->mutability = mtb;
	    this->kind = TypeKind::Integer;
	}

	Type (TypeKind kind, lex::Location loc)
	    : kind (kind), loc (loc) {}
    };
    
    using TypePtr = std::unique_ptr<Type>;

#define make_type(...) std::make_unique<tuff::ast::Type>(__VA_ARGS__)
} // tuff::ast
