#include "compiler.h"

#include <string.h>

#include "args.h"
#include "array.h"
#include "def.h"
#include "module.h"

void *
tuff_alloc (CompileSession *sess, size_t size) {
    struct Alloc alloc;
    alloc.size = size;
    alloc.ptr  = malloc (size);
    arr_push (&sess->allocator.allocations, alloc);
    return alloc.ptr;
}

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
    return id;
}

Module *
get_module (CompileSession *sess, ModuleId id) {
    return &arr_get (&sess->loaded_modules, id);
}
