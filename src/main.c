
#include "compiler.h"

int
main (int argc, char *argv[]) {
    CompileSession sess = open_session (argc, argv);
    sess.diag_collector = make_diag_collector (&sess);
    
    const char *input_file = sess.args.input_file;
    load_module (&sess, input_file);
    return 0;
}
