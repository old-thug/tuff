#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "array.hh"
#include "args.hh"
#include "error/error.hh"
#include "unit/module.hh"

namespace tuff {

    struct Allocator {
	struct Alloc {
	    void *ptr;
	    size_t size;
	};
	
	Array<Alloc> allocator;
	void *
	alloc (size_t size) {
	    Alloc a;
	    a.size = size;
	    a.ptr  = malloc (size);
	    auto p = a.ptr;
	    this->allocator.push (a);
	    return p;
	}
	~Allocator () {
	    for (auto alloc: this->allocator) {
		if (alloc.ptr) {
		    free (alloc.ptr);
		}
	    }
	}
    public:
	template<typename T> T*
	make(T obj) {
	    T *mem = (T*)alloc (sizeof (T));
	    memcpy ((void*)mem, (void*)&obj, sizeof (T));
	    return mem;
	}

	template<typename T> T*
	make() {
	    T *mem = (T*)alloc (sizeof (T));
	    return mem;
	}

	template<typename T, typename ...Args> T*
	make(Args ...args) {
	    T *mem = (T*)alloc (sizeof (T));
	    *mem = T(args...);
	    return mem;
	}
    };
    
    using ModuleId = uint64_t;
    struct CompileSession {
	Array<unit::Module> loaded_modules;
	Allocator _allocator;
	args::Args args;
	error::DiagCollector diag_collector;

    public:
	CompileSession (args::Args args) : loaded_modules(), _allocator(), args(args) {}
	ModuleId load_module (const char *path);
	unit::Module *get_module( ModuleId id);
	error::DiagCollector *dcollector ();
	Allocator *allocator ();
    };
}
