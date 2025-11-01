#pragma once

namespace tuff::unit {
    // A module represents a single file during compilation.
    struct Module {
        const char *file_path;
        const char *file_content;
        const char *module_name;
	// TODO: add symbols discovered in this module
    public:
	void set_module_name (const char *name);
	Module (const char *path);
	const char *source () const;
    };
}
