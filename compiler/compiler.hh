#pragma once

#include <unordered_map>
#include <memory>

#include "./args.hh"
#include "unit/module.hh"

namespace tuff {
    struct CompileSession {
	args::Args args;
	std::unordered_map<std::string,
			   std::unique_ptr<unit::Module>> loaded_modules;

    public:
	unit::Module *load_module(const std::string& path);
    };
}
