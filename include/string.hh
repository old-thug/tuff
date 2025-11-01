#pragma once
#include "array.hh"
#include "def.hh"

NOTE (old-thug, "incomplete header: do not use")

struct String {
private:
    Array<char> buffer;
public:
    String (): buffer () {}
    String (const char * s)
	: buffer () {
	while (s) {
	    this->buffer.push((char)*s);
	    s++;
	}
    }

    String (const String& other)
	: buffer (other.buffer) {}
    
    ~String () {
	this->buffer.~Array();
    }
};
