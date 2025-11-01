#pragma once

#include "array.hh"
#include "ast/ast.hh"
#include "map.hh"

namespace tuff::unit {
    // A module represents a single file during compilation.
    struct Module {
	using Items = Array<ast::NodePtr>;
	
        const char *file_path;
        const char *file_content;
        const char *module_name;
	Items items;
	StringMap<void*> symbols;
	// TODO: add symbols discovered in this module
    public:
	void set_module_name (const char *name);
	Module (const char *path);
	Module () = default;
	const char *source () const;
    };
}
