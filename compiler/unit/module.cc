#include "unit/module.hh"

#include <cstdio>
#include <cstdlib>

namespace tuff::unit {
    Module::Module(const char *path)
    {
	this->file_path = path;
	// File existence should be ensured before calling this ctor.
	FILE *file = fopen (path, "rb");
	if (file) {
	    fseek (file, 0, SEEK_END);
	    auto size = ftell(file);
	    rewind (file);

	    auto buf = (char *)malloc (size+1);
	    fread (buf, 1, size, file);
	    buf[size] = 0;
	    this->file_content = buf;
	}
	fclose (file);
    }

    const char *Module::source () const
    {
	return this->file_content;
    }
}
