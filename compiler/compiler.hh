#pragma once

#include <cstdint>

#include "array.hh"
#include "args.hh"
#include "error/error.hh"
#include "unit/module.hh"

namespace tuff {
    using ModuleId = uint64_t;
    struct CompileSession {
	Array<unit::Module> loaded_modules;
	args::Args args;
	error::DiagCollector diag_collector;

    public:
	CompileSession (args::Args args) : loaded_modules(), args(args) {}
	ModuleId load_module (const char *path);
	unit::Module *get_module( ModuleId id);
	error::DiagCollector *dcollector ();
    };
}
