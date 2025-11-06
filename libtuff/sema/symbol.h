#ifndef _TUFF_SEMA_SYMBOL_H_
#define _TUFF_SEMA_SYMBOL_H_

#include "sema/prototype.h"
#include "sv.h"
#include "map.h"

typedef struct Symbol Symbol;
typedef enum {
    SYM_Function,
    SYM_Variable,
    SYM_Module,
} SymbolKind;

typedef struct {
    StringView name;
    FunctionPrototype prototype;
} FunctionSymbol;

typedef struct {
    StringView name;
    TypePtr type;
    Mutability mutability;
} VariableSymbol;

struct Symbol {
    SymbolKind kind;
    Location   locus;
    union {
	FunctionSymbol function;
	VariableSymbol variable;
    };
};

typedef struct {
    HashMap inner;
} SymbolMap;

typedef Symbol* SymbolPtr;

SymbolMap
make_sym_map ();
SymbolPtr
make_func_symbol (Allocator *allocator, Location locus,
		  FunctionSymbol func);
SymbolPtr
make_var_symbol (Allocator *allocator, Location locus,
		 VariableSymbol var);

void
put_symbol_to_map (SymbolMap *map, const char *name, SymbolPtr sym);
SymbolPtr
get_symbol_from_map (SymbolMap *map, const char *name);

#endif // _TUFF_SEMA_SYMBOL_H_
