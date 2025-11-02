#include "diag/diag.h"
#include "compiler.h"
#include <stdarg.h>
#include <stdio.h>
#include <vadefs.h>

Diagnostic*
diagnostic_at (DiagnosticCollector *dc, DiagSeverity sev, Location locus, const char *fmt, ...) {
    Diagnostic diag = {0};
    diag.collector = dc;
    diag.severity  = sev;

    va_list args;
    va_start (args, fmt);

    size_t size = vsnprintf (NULL, 0, fmt, args);
    va_end (args);
    va_start (args, fmt);

    char *str = (char *)tuff_alloc (&dc->sess->allocator, size + 1);
    vsnprintf (str, size + 1, fmt, args);
    va_end (args);

    diag.label.message = str;
    diag.label.locus   = locus;
    

    arr_push (&dc->diagnostics, diag);
    return &arr_last (&dc->diagnostics);
}

void
diag_note (Diagnostic *diag, Location locus, const char *fmt, ...) {
    Label label;
    label.locus = locus;
    va_list args;
    va_start (args, fmt);
    
    size_t size = vsnprintf (NULL, 0, fmt, args);
    va_end (args);
    va_start (args, fmt);

    char *str = (char *)tuff_alloc (&diag->collector->sess->allocator, size + 1);
    vsnprintf (str, size + 1, fmt, args);
    va_end (args);

    label.message = str;
    arr_push (&diag->secondary_labels, label);
}
