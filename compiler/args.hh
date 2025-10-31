#pragma once

#include <string>
#include <optional>

namespace tuff::args {
    struct Args {
	std::string input_file;
	std::optional<std::string> output_file;
	int argc;
	char **argv;
	
	Args(int argc, char **argv);
	bool parse();

    private:
	bool default_selected = true;
	std::string next_arg();
    };
} // args
