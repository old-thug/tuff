#include <cstdlib>

#include "./args.hh"

int main(int argc, char *argv[])
{
    tuff::args::Args args (argc, argv);

    if (!args.parse ()) {
	return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
