
#include "args.h"
#include "libtuff/compiler.h"

int
main (int argc, char *argv[]) {
    Args args = parse_args (argc, argv);
    CompileSession sess = open_session ();
    sess.diag_collector = make_diag_collector (&sess);
    
    const char *input_file = args.input_file;
    ModuleId main_mod = load_module (&sess, input_file);
    return 0;
}
