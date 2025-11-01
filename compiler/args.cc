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
	this->output_file = nullptr;
    }

    const char * Args::next_arg()
    {
	argc--;
	return *argv++;
    }
    
    bool Args::parse()
    {
	// TODO: actually parse the arguments;
	next_arg ();		// skip program name
	if (this->argc == 0) {
	    printf ("usage: tuffc <input>\n");
	    exit (1);
	}
	this->input_file = next_arg ();
	if (!fopen(this->input_file, "rb")) {
	    printf ("error: no such file `%s`\n",
		    this->input_file);
	    exit (1);
	}
	return true;
    }
}
