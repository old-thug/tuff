#include "./compiler.hh"

#include "error/error.hh"
#include "lex/loc.hh"
#include "parse/parser.hh"
#include "def.hh"
#include "unit/module.hh"

namespace tuff {
    void
    print_loc (lex::Location loc, CompileSession *sess) {
	unit::Module *mod = sess->get_module (loc.module_id);
	printn ("%s:%zu:%zu",
		mod->file_path,
		loc.first_line, loc.begin);
    }

    TODO (old-thug, "fix this mess");
    void
    report_diag (error::Diag *diag, CompileSession *sess) {
	print_loc (diag->label.loc, sess);
	printn (": error: ");
	printn ("%s\n", diag->label.message);
	for (auto lbl: diag->labels) {
	    print_loc (lbl.loc, sess);
	    printn (": ");
	    switch (lbl.level) {
	    case tuff::error::Diag::Level::Note:
		printn ("note"); break;
	    case tuff::error::Diag::Level::Error:
		printn ("error"); break;
	    case tuff::error::Diag::Level::Warning:
		printn ("warning"); break;		
	    default: todo ();
	    }

	    printn (": %s\n", lbl.message);
	}
    }
    
    void
    report_diags (CompileSession *sess) {
	for (auto diag: sess->diag_collector.diags) {
	    report_diag(&diag, sess);
	}
    }
    
    tuff::ModuleId
    CompileSession::load_module (const char *path)
    {	
	auto new_mod = unit::Module (path);
	auto id = this->loaded_modules.len ();
	this->loaded_modules.push (new_mod);
	auto parser = parse::Parser (id, this);
	parser.parse();

	report_diags (this);
	return id;
    }

    unit::Module*
    CompileSession::get_module (ModuleId id)
    {
	if (id >= this->loaded_modules.len ()) return nullptr;
	return this->loaded_modules.get_ptr (id);
    }

    error::DiagCollector*
    CompileSession::dcollector () {
	return &this->diag_collector;
    }
}
