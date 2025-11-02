#include <stdio.h>

#include "module.h"
#include "array.h"
#include "compiler.h"
#include "sv.h"
#include "def.h"

static void
generate_source_lines (Module *mod) {
    const char *start = mod->source;
    const char *source = start;
    while (*source) {
	if (*source == '\n') {
	    int len = source - start;
	    arr_push (&mod->source_lines, sv_init (start, len));
	    start = source + 1;
	}
	source++;
    }

    if (source != start) {
	int len = source - start;
	arr_push (&mod->source_lines, sv_init (start, len));
    }
}

bool
open_module (const char *source_path, Module *mod, CompileSession *sess) {
    FILE *file = fopen (source_path, "rb");
    if (!file) return false;

    fseek (file, 0, SEEK_END);
    size_t size = ftell (file);
    rewind (file);

    mod->module_name = NULL;
    mod->source = tuff_alloc (&sess->allocator, size + 1);
    fread ((char *)mod->source, 1, size, file);
    ((char *)mod->source)[size] = 0;
    generate_source_lines (mod);
    return true;
}
