#include "sema/symbol.h"

SymbolMap
make_sym_map () {
    SymbolMap map = {0};
    map.inner = hashmap_make ();
    return map;
}

static SymbolPtr
make_symbol (Allocator *allocator, Location locus, SymbolKind kind) {
    SymbolPtr sym = tuff_alloc (allocator, sizeof (Symbol));
    sym->kind = kind;
    sym->locus = locus;
    return sym;
}

SymbolPtr
make_func_symbol (Allocator *allocator, Location locus,
		  FunctionSymbol func) {
    SymbolPtr sym = make_symbol (allocator, locus, SYM_Function);
    sym->function = func;
    return sym;
}

SymbolPtr
make_var_symbol (Allocator *allocator, Location locus,
		 VariableSymbol var) {
    SymbolPtr sym = make_symbol (allocator, locus, SYM_Function);
    sym->variable = var;
    return sym;
}

void
put_symbol_to_map (SymbolMap *map, const char *name, SymbolPtr sym) {
    hashmap_put (&map->inner, name, (void*)sym);
}

SymbolPtr
get_symbol_from_map (SymbolMap *map, const char *name) {
    return (SymbolPtr)hashmap_get (&map->inner, name);
}
