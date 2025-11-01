#pragma once

#include "array.hh"
#include "lex/loc.hh"
#include <cstdarg>
#include <cstdio>

#define MAX_DIAG_COUNT 20

namespace tuff::error {    
    struct Diag {
	enum Level {
	    Error,
	    Warning,
	    Note,
	};
	
	struct Label {
	    Diag::Level level = Diag::Level::Note;
	    lex::Location loc;
	    const char    *message;
	    const char    *note;
	    bool           loc_set = true;

	    Label (lex::Location loc, const char *msg, Level lvl)
		: level (lvl), loc (loc), message (msg) {}
	} label;
	
	Array<Diag::Label> labels;

	Diag (lex::Location loc, const char *msg, Diag::Level lvl = Level::Error)
	    : label (loc, msg, lvl) {}

	Diag *
	note (lex::Location loc, const char *fmt, ...) {
	    va_list args;
	    va_start (args, fmt);
	    
	    auto size = vsnprintf (NULL, 0, fmt, args);
	    va_start (args, fmt);

	    char *str = (char *)malloc (size + 1);
	    vsprintf (str, fmt, args);
	    str[size] = 0;

	    this->labels.push (Label (loc, fmt, Level::Note));
	    return this;
	}
    };

    struct DiagCollector {
	size_t max_diag = MAX_DIAG_COUNT;
	Array<Diag>       diags;

	DiagCollector ()
	    : diags () {}

	DiagCollector (size_t max)
	    : max_diag (max), diags () {}

	Diag*
	error_at (lex::Location loc, const char *fmt, ...) {
	    va_list args;
	    va_start (args, fmt);
	    
	    auto size = vsnprintf (NULL, 0, fmt, args);
	    va_start (args, fmt);

	    char *str = (char *)malloc (size + 1);
	    vsprintf (str, fmt, args);
	    str[size] = 0;

	    this->diags.push(Diag(loc, str));
	    return this->diags.get_ptr(this->diags.len() - 1);
	}
    };
} // tuff::error
