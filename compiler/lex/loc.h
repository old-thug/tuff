#ifndef _TUFF_LOC_H_
#define _TUFF_LOC_H_

#include <stddef.h>
#include <stdint.h>

#include "def.h"

typedef uint64_t ModuleId;

typedef struct {
    size_t first_line;
    size_t last_line;
    size_t first_col;
    size_t last_col;
    ModuleId module_id;
} Location;

inlined Location
new_locus(size_t first_line, size_t last_line, size_t first_col, size_t last_col, ModuleId id) {
    return (Location) { first_line, last_line, first_col, last_col, id };
}

#endif // _TUFF_LOC_H_
