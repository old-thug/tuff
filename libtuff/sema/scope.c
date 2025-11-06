#include "sema/scope.h"

ScopePtr
make_scope (Allocator * allocator, ScopeFlags flags, ScopePtr parent) {
    ScopePtr scope = tuff_alloc (allocator, sizeof (Scope));
    scope->symbols = make_sym_map ();
    scope->parent = parent;
    scope->flags  = flags;
    return scope;
}

SymbolPtr
find_in_scope (ScopePtr scope, const char *name) {
    return get_symbol_from_map (&scope->symbols, name);
}

SymbolPtr
eagerly_find_in_scope (ScopePtr scope, const char *name) {
    ScopePtr cur = scope;
    while (cur) {
	SymbolPtr sym;
	if ((sym = find_in_scope (cur, name)) != NULL) return sym;
	cur = cur->parent;
    }
    return NULL;
}

void
add_to_scope (ScopePtr scope, const char *name, SymbolPtr sym) {
    put_symbol_to_map (&scope->symbols, name, (void*)sym);
}
