#include "args.h"
#include "def.h"

const char *
shift_arg (int *c, char ***v) {
    (*c)--;
    return (const char *) *(*v)++;
}

Args
parse_args (int argc, char **argv) {
    Args args;
    const char *pn = shift_arg (&argc, &argv);	/* program name */
    if (!argc) {
	todo ();
    }
    args.input_file = shift_arg (&argc, &argv);
    return args;
}
