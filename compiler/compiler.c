#include "compiler.h"

#include <string.h>

#include "args.h"
#include "array.h"
#include "def.h"
#include "diag/diag.h"
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
open_session (int argc, char **argv) {
    CompileSession sess = {0};
    sess.args = parse_args (argc, argv);
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
    if (!open_module (file_path, &mod, sess)) todo ();
    arr_push (&sess->loaded_modules, mod);
    Parser parser = open_parser (sess, id);
    parser_parse (&parser);
    return id;
}

Module *
get_module (CompileSession *sess, ModuleId id) {
    return &arr_get (&sess->loaded_modules, id);
}
