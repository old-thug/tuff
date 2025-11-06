#ifndef _TUFF_SEMA_ERROR_H_
#define _TUFF_SEMA_ERROR_H_

#define sym_error(d, sev, loc, tag, fmt, ...) diagnostic_at (d,		\
							     sev,	\
							     loc,	\
							     tag,	\
							     fmt,	\
							     ##__VA_ARGS__)

inlined Diagnostic*
sema_err_func_redefinition (DiagnosticCollector *collector,
			    Location location,
			    Location first_def,
			    StringView name) {
    Diagnostic *err = sym_error (collector, DIAG_Error, location,
				 NULL, "redefinition of function `%.*s`", LIT (name));
    diag_note (err, first_def, "here", "first defined here");
    return err;
}


#endif // _TUFF_SEMA_ERROR_H_
