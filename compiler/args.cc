#include "args.hh"

#include <cstdio>
#include <cstdlib>

#define DEFAULT_INPUT_FILE "main.tuf"

namespace tuff::args {
    Args::Args(int argc, char **argv)
    {
	this->argc = argc;
	this->argv = argv;
	this->input_file = DEFAULT_INPUT_FILE;
	this->output_file = std::nullopt;
    }

    std::string Args::next_arg()
    {
	argc--;
	return (*argv)++;
    }
    
    bool Args::parse()
    {
	// TODO: actually parse the arguments;
	next_arg ();		// skip program name
	if (!this->argc) {
	    std::printf ("usage: tuffc <input>");
	    std::exit (1);
	}
	this->input_file = next_arg ();
	return true;
    }
}
