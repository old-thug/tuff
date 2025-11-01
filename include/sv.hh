#pragma once

#include <cstring>

#define LIT(sv) (int)sv.len (), sv.data ()

struct StringView {
private:
    const char *_data;
    size_t _len;
public:
    StringView ()
	: _data (""), _len (0) {}

    StringView (const char *_b)
	: _data (_b), _len (strlen (_b)) {}
    
    StringView (const char *_b, size_t _n)
	: _data (_b), _len (_n) {}
    
    StringView
    substr (size_t n, size_t m) {
	return StringView (&this->_data[n], m);
    }

    const char *
    data () {
	return this->_data;
    }

    bool
    starts_with (const char *s) {
	return !strncmp(this->_data, s, strlen (s));
    }
    
    size_t len () { return this->_len; }

    bool operator==(const char *s)
    {
	if (strlen (s) != this->_len) return false;
	return !strncmp (this->_data, s, this->_len);
    }
};
