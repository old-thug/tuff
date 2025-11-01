#pragma once

namespace tuff::args {
    struct Args {
	const char *input_file;
	const char *output_file;
	int argc;
	char **argv;
	
	Args(int argc, char **argv);
	bool parse();

    private:
	bool default_selected = true;
	const char *next_arg();
    };
} // args
