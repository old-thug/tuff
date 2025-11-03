#ifndef _TUFF_DIAG_H_
#define _TUFF_DIAG_H_

#include "lex/loc.h"
#include "array.h"
#include <stdbool.h>

typedef enum {
    DIAG_Error,
    DIAG_Warning,
    DIAG_Note,
} DiagSeverity;

static const char *diag_spelling[] = {
    [DIAG_Error]	= "error",
    [DIAG_Warning]	= "warning",
    [DIAG_Note]		= "note",
};

typedef struct Label {
    const char *message;
    const char *tag;
    Location    locus;
} Label;

typedef struct DiagnosticCollector DiagnosticCollector;
typedef struct CompileSession CompileSession;
typedef struct {
    DiagnosticCollector *collector;
    DiagSeverity severity;
    Label label;
    const char *sev_str;
    // 4 | func foo foo () void {
    //   |          ^^^ <tag>
    const char *tag;
    arr_of (Label) secondary_labels;
} Diagnostic;

typedef struct DiagnosticCollector {
    CompileSession *sess;	/* for internal allocations */
    arr_of (Diagnostic) diagnostics;
    int warning_count;
    int error_count;
} DiagnosticCollector;

inlined DiagnosticCollector
make_diag_collector (CompileSession *sess) {
    DiagnosticCollector d = {0};
    d.sess = sess;
    return d;
}

Diagnostic*
diagnostic_at (DiagnosticCollector *dc, DiagSeverity sev, Location locus, const char *tag, const char *fmt, ...);

void
diag_note (Diagnostic *diag, Location locus, const char *tag, const char *fmt, ...);

void
print_diagnostic (const Diagnostic *diag);

#endif // _TUFF_DIAG_H_
