#ifndef __TUFF_MODULE_H_
#define  __TUFF_MODULE_H_

#include <stdint.h>
#include <stdbool.h>

#include "ast/node.h"
#include "array.h"
#include "sv.h"

typedef uint64_t ModuleId;
typedef struct Module {
    const char *module_name;
    const char *source;
    const char *source_path;
    arr_of(StringView) source_lines;
    NodeList ast;
} Module;

typedef Module* ModulePtr;
typedef struct CompileSession CompileSession;
bool open_module (const char *source_path, Module *mod, CompileSession *sess);

#endif //  __TUFF_MODULE_H_
