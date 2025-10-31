#pragma once

#include <string>

namespace tuff::unit {
    // A module represents a single file during compilation.
    struct Module {
	std::string file_path;
	std::string file_content;
	// TODO: add symbols discovered in this module

	Module(const std::string& path);
    };
}
