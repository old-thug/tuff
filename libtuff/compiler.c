#include "compiler.h"

#include <string.h>

#include "array.h"
#include "def.h"
#include "diag/diag.h"
#include "lex/token.h"
#include "module.h"
#include "parse/parser.h"

void *
tuff_alloc (Allocator *allocator, size_t size) {
    struct Alloc alloc;
    alloc.size = size;
    alloc.ptr  = malloc (size);
    memset (alloc.ptr, 0, size);
    arr_push (&allocator->allocations, alloc);
    return alloc.ptr;
}

// Don't initialize diag_collector here – needs pointer to session.
CompileSession
open_session () {
    CompileSession sess = {0};
    return sess;
}

ModuleId
load_module (CompileSession *sess, const char *file_path) {
    for (size_t n = 0; n < arr_len (&sess->loaded_modules); n++) {
	Module *mod = &arr_get (&sess->loaded_modules, n);
	if (!strcmp (mod->source_path, file_path)) {
	    return n;
	}
    }
    ModuleId id = arr_len (&sess->loaded_modules);
    Module mod = {0};
    if (!open_module (file_path, &mod, sess)) todo ("failed to open module");
    arr_push (&sess->loaded_modules, mod);
    Parser parser = open_parser (sess, id);
    
#if 0
    while (!is_at_end (&parser)) {
	Token tok = next_token (&parser);
	println ("TOKEN: %s", token_name (tok.id));
    }
#else
    parser_parse (&parser);
#endif
    return id;
}

ModulePtr
get_module (CompileSession *sess, ModuleId id) {
    return &arr_get (&sess->loaded_modules, id);
}
