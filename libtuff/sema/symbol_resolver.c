#include "sema/symbol_resolver.h"
#include "sema/scope.h"
#include "sema/sema_error.h"

typedef struct {
    CompileSession *sess;
    DiagnosticCollector *diag_collector;
    Allocator *allocator;
    ScopePtr root_scope;
    ScopePtr current_scope;
    ModulePtr module;

    uint64_t symbol_id_counter;
} SymContext;

void
sym_check_node (SymContext *ctx, NodePtr node, bool first_pass);
void
sym_check_function (SymContext *ctx, NodePtr node, bool first_pass);
void
sym_check_declaration (SymContext *ctx, NodePtr node, bool first_pass);
void
sym_check_block (SymContext *ctx, NodePtr node);

SymbolPtr
get_global_symbol (SymContext *ctx, StringView name) {
    const char *key = sv_get (name);
    SymbolPtr sym = find_in_scope (ctx->root_scope, key);
    free ((void*)key);
    return sym;
}

SymbolPtr
get_local_symbol (SymContext *ctx, StringView name) {
    const char *key = sv_get (name);
    SymbolPtr sym = eagerly_find_in_scope (ctx->current_scope, key);
    free ((void*)key);
    return sym;
}

SymbolPtr
get_local_symbol_shallow (SymContext *ctx, StringView name) {
    const char *key = sv_get (name);
    SymbolPtr sym = find_in_scope (ctx->current_scope, key);
    free ((void*)key);
    return sym;
}

void
add_global_symbol (SymContext *ctx, StringView name, SymbolPtr sym) {
    const char *key = sv_get (name);
    add_to_scope (ctx->current_scope, key, sym);
    free ((void*)key);
}

void
enter_scope (SymContext *ctx, ScopeFlags flags) {
    ctx->current_scope = make_scope (ctx->allocator, flags, ctx->current_scope);
}

ScopePtr
leave_scope (SymContext *ctx) {
    ScopePtr prev = ctx->current_scope;
    ctx->current_scope = (prev->parent)? prev->parent: ctx->root_scope;
    return prev;
}

void
run_symbol_resolver (CompileSession *sess, ModuleId id) {
    SymContext sc = {0};
    sc.sess = sess;
    sc.diag_collector = &sess->diag_collector;
    sc.allocator = &sess->allocator;
    sc.root_scope = make_scope (sc.allocator, SCOPE_Global | SCOPE_Regular, NULL);
    sc.current_scope = sc.root_scope;
    sc.module = get_module (sess, id);
    sc.symbol_id_counter = 1;

    for (int n = 0; n < arr_len (&sc.module->ast); n++) {
	NodePtr item = arr_get (&sc.module->ast, n);
	sym_check_node (&sc, item, true);
    }

    for (int n = 0; n < arr_len (&sc.module->ast); n++) {
	NodePtr item = arr_get (&sc.module->ast, n);
	sym_check_node (&sc, item, false);
    }
}

void
sym_check_node (SymContext *ctx, NodePtr node, bool first_pass) {
    switch (node->kind) {
    case NODE_Func:  sym_check_function (ctx, node, first_pass); break;
    case NODE_Block: sym_check_block (ctx, node); break;
    case NODE_Declare: sym_check_declaration (ctx, node, first_pass); break;
    default: todo ();
    }
}

void
sym_check_function (SymContext *ctx, NodePtr node, bool first_pass) {
    FunctionItem *func = &node->function;
    if (first_pass) {
	Symbol *sym = get_global_symbol (ctx, func->name.value);
	if (sym) {
	    sema_err_func_redefinition (ctx->diag_collector, func->name.locus,
					sym->locus, func->name.value);
	    return;
	}

	FunctionSymbol fsym = {0};
	sym = make_func_symbol (ctx->allocator, func->name.locus, fsym);
	add_global_symbol (ctx, func->name.value, sym);
	node->scope = ctx->root_scope;
	node->sym = sym;
    } else {
	NodePtr expr = func->expression;
	enter_scope (ctx, SCOPE_Returnable | SCOPE_Regular);
	sym_check_node (ctx, expr, false);
	Scope *scope = leave_scope (ctx);
    }
}

void
sym_check_block (SymContext *ctx, NodePtr node) {
    BlockExpr *block = &node->block;
    for (int n = 0; n < arr_len (&block->expressions); n++) {
	NodePtr expr = arr_get (&block->expressions, n);
	sym_check_node (ctx, expr, false);
    }
}

void
sym_check_declaration (SymContext *ctx, NodePtr node, bool first_pass) {
    DeclareExpr *decl = &node->declare;
    StringView name = decl->ident.value;
    SymbolPtr sym = get_local_symbol (ctx, name);
    node->scope = ctx->current_scope;
    sym_check_node (ctx, decl->expression, first_pass);
}
