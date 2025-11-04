#include "diag/diag.h"
#include "compiler.h"
#include "module.h"
#include <stdarg.h>
#include <stdio.h>
#include <vadefs.h>

Diagnostic*
diagnostic_at (DiagnosticCollector *dc, DiagSeverity sev, Location locus, const char *tag, const char *fmt, ...) {
    Diagnostic diag = {0};
    diag.collector = dc;
    diag.severity  = sev;
    diag.label.tag = tag;
    if (sev == DIAG_Error) dc->error_count++;
    diag.sev_str   = diag_spelling[sev];
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
diag_simple_note (Diagnostic *diag, const char *fmt, ...) {
   Label label;
   label.carries_locus = false;;
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

void
diag_note (Diagnostic *diag, Location locus, const char *tag, const char *fmt, ...) {
    Label label;
    label.locus = locus;
    label.tag  = tag;
    label.carries_locus = true;
    
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

void
print_location (Location loc, const Module *mod) {
    eprintln(" -->: %s:%zu:%zu", mod->source_path, loc.first_line, loc.first_col);
}

void
print_source_line(const Label *lbl, const Module *mod, Location loc) {
    if (loc.first_line == 0 || loc.first_line > arr_len(&mod->source_lines)) return;

    StringView line = arr_get(&mod->source_lines, loc.first_line - 1);
    eprintln("      |");
    eprint(" %4zu | ", loc.first_line);
    eprintln("%.*s", LIT (line));
    eprint("      | ");
    for (size_t i = 1; i < loc.first_col; i++) eprint(" ");
    size_t len = loc.last_col > loc.first_col ? loc.last_col - loc.first_col : 1;
    for (size_t i = 0; i < len; i++) eprint("^");
    if (lbl && lbl->tag) {
	eprint (" %s", lbl->tag);
    }
    eprintln();
}


void
print_diagnostic (const Diagnostic *diag) {
    const Module *mod = get_module (diag->collector->sess, diag->label.locus.module_id);
    eprintln("%s: %s", diag->sev_str, diag->label.message);
    print_location(diag->label.locus, mod);
    print_source_line(&diag->label, mod, diag->label.locus);
    for (size_t i = 0; i < arr_len(&diag->secondary_labels); i++) {
        Label *label = &arr_get(&diag->secondary_labels, i);
        eprintln("note: %s", label->message);
	if (label->carries_locus) {
	    print_location(label->locus, mod);
	    print_source_line(label, mod, label->locus);
	}
    }
}
