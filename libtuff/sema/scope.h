#ifndef _TUFF_SEMA_SCOPE_H_
#define _TUFF_SEMA_SCOPE_H_

#include "map.h"
#include "sema/symbol.h"

typedef enum {
    SCOPE_Returnable = 1 << 0,
    SCOPE_Continuable = 1 << 1,
    SCOPE_Breakable = 1 << 2,
    SCOPE_Regular = 1 << 3,
    SCOPE_Global  = 1 << 4,	/* is this needed? */
} ScopeFlags;

typedef struct Scope {
    struct Scope *parent;
    SymbolMap     symbols;
    ScopeFlags    flags;
} Scope;
typedef Scope* ScopePtr;

ScopePtr
make_scope (Allocator *, ScopeFlags flags, ScopePtr parent);

// Searches for `name` in `scope`.
// returns `NULL` if not found.
SymbolPtr
find_in_scope (ScopePtr scope, const char *name);

// Searches for `name` in `scope` and recursively, `scope`'s parents.
// returns null if not found
SymbolPtr
eagerly_find_in_scope (ScopePtr scope, const char *name);

void
add_to_scope (ScopePtr scope, const char *name, SymbolPtr sym);

#endif // _TUFF_SEMA_SCOPE_H_
