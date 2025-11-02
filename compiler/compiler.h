#ifndef __TUFF_COMPILER_H_
#define __TUFF_COMPILER_H_

#include <stddef.h>

#include "args.h"
#include "array.h"
#include "diag/diag.h"
#include "module.h"

#define tuff_inline static inline

struct Alloc {
    size_t size;
    void  *ptr;
};

typedef struct {
    arr_of(struct Alloc) allocations;
} Allocator;

typedef struct CompileSession {
    Args args;
    Allocator allocator;
    DiagnosticCollector diag_collector;
    arr_of (Module) loaded_modules;
} CompileSession;

void *
tuff_alloc (Allocator *, size_t size);

CompileSession
open_session (int argc, char **argv);

ModuleId
load_module (CompileSession *sess, const char *file_path);

Module *
get_module (CompileSession *sess, ModuleId id);

#endif // __TUFF_COMPILER_H_
