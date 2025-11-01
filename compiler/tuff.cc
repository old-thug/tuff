#include <cstdlib>

#include "./args.hh"
#include "./compiler.hh"
#include "./lex/loc.hh"

int main(int argc, char *argv[])
{
    tuff::args::Args args (argc, argv);
    if (!args.parse ()) {
	return EXIT_FAILURE;
    }
    
    tuff::CompileSession session (args);
    auto mod = session.load_module (args.input_file);
    
    return EXIT_SUCCESS;
}
