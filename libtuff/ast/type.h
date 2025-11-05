#ifndef _TUFF_NODE_TYPE_H_
#define _TUFF_NODE_TYPE_H_

#include "compiler.h"
#include "lex/loc.h"
typedef struct Type Type;
typedef Type* TypePtr;

typedef enum {
    TYPE_Void,
    TYPE_Pending,
    TYPE_Integer,
    TYPE_String,
    TYPE_Pointer,
    TYPE_Reference,
    TYPE_Optional
} TypeKind;

typedef enum {
    MUTABILITY_Mutable,
    MUTABILITY_Immutable,
} Mutability;

typedef enum {
    INTSIZE_8,
    INTSIZE_16,
    INTSIZE_32,
    INTSIZE_64
} IntegerSize;

typedef enum {
    INTSIGN_Signed,
    INTSIGN_Unsigned
} IntegerSign;

typedef struct {
    TypeKind kind;
    union {
	struct {
	    IntegerSign sign;
	    IntegerSize size;
	} integer;

	struct {
	    TypePtr inner;
	} pointer;

	struct {
	    TypePtr inner;
	} reference;

	struct {
	    TypePtr inner;
	} optional;
    };
} CanonicalType;

typedef struct Type {
    const CanonicalType *base;
    Mutability mutability;
    Location locus;
} Type;

CanonicalType*
get_int_type (IntegerSign sign, IntegerSize size);
CanonicalType*
get_void_type ();

TypePtr
make_int_type (Allocator *allocator, IntegerSign sign, IntegerSize size, Location locus);
TypePtr
make_void_type (Allocator *allocator, Location locus);
TypePtr
make_pending_type (Allocator *allocator, Location locus);

#endif // _TUFF_NODE_TYPE_H_
