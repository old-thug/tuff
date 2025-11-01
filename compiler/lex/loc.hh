#pragma once

#include <cstdint>

typedef uint64_t ModuleId;
namespace tuff::lex {
    struct Location {
	size_t first_line, last_line;
	size_t begin, end;
        ModuleId module_id;

	Location () = default;
	Location (size_t first_line, size_t last_line,
		 size_t begin, size_t end,
		 ModuleId module_id)
	    : first_line (first_line), last_line (last_line),
	      begin (begin), end (end), module_id (module_id) {}

	Location
	merge (const Location& other) const {
	    return Location (this->first_line, other.last_line,
			     this->begin, other.end,
			     this->module_id);
	}
    };
} // tuff::lex
