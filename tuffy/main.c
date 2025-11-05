
#include "libtuff/compiler.h"
#include "def.h"
#include "array.h"

int
main (int argc, char *argv[]) {
    if (argc <=  1) {
	eprintln ("usage: %s <input>", *argv);
	return 1;
    }
    TODO (old-thug, "parse actual arguments");
    const char *input_file = argv[1];
    CompileSession sess = open_session ();
    sess.diag_collector = make_diag_collector (&sess);
    ModuleId main_mod = load_module (&sess, input_file);
    if (arr_len (&sess.diag_collector.diagnostics)) {
	for (int n = 0; n < arr_len (&sess.diag_collector.diagnostics); n++) {
	    Diagnostic *diag = &arr_get (&sess.diag_collector.diagnostics, n);
	    print_diagnostic (diag);
	}
    }
    return 0;
}
