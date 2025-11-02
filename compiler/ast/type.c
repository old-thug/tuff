#include "ast/type.h"
#include "compiler.h"

static
CanonicalType t_int32 = {
    .integer = {
	.sign = INTSIGN_Signed,
	.size = INTSIZE_32,
    },
    .kind = TYPE_Integer,
};

static
CanonicalType t_pending = {
    .kind = TYPE_Pending,
};

static
CanonicalType t_void = {
    .kind = TYPE_Void,
};

CanonicalType*
get_int_type (IntegerSign sign, IntegerSize size) {
    if (sign == INTSIGN_Signed && size == INTSIZE_32) return &t_int32;
    else todo ();
}

CanonicalType*
get_void_type () {
    return &t_void;
}

TypePtr
make_int_type (Allocator *allocator, IntegerSign sign, IntegerSize size, Location locus) {
    TypePtr ty = (TypePtr)tuff_alloc (allocator, sizeof (Type));
    ty->base = get_int_type (sign, size);
    ty->mutability = MUTABILITY_Mutable;
    ty->locus = locus;
    return ty;
}

TypePtr
make_void_type (Allocator *allocator, Location locus) {
    TypePtr ty = (TypePtr)tuff_alloc (allocator, sizeof (Type));
    ty->base = get_void_type ();
    ty->mutability = MUTABILITY_Immutable;
    ty->locus = locus;
    return ty;
}

TypePtr
make_pending_type (Allocator *allocator, Location locus) {
    TypePtr ty = (TypePtr)tuff_alloc (allocator, sizeof (Type));
    ty->base   = &t_pending;
    ty->locus  = locus;
    ty->mutability = MUTABILITY_Immutable;
    return ty;
}
