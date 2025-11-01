#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

template <typename T> struct Array {
private:
    T *buffer;
    size_t count, capacity;
    size_t INIT_CAP = 4;
public:
    Array ()
    {
	this->buffer = (T*)std::malloc (sizeof(T)*INIT_CAP);
	this->capacity = INIT_CAP;
	this->count = 0;
    }

    void push (T&& item)
    {
	if (this->count >= this->capacity) {
	    this->capacity *= 2;
	    this->buffer = (T*)std::realloc (this->buffer, sizeof(T)*this->capacity);
	}
	this->buffer[this->count ++] = std::move (item);
    }

    void push (T& item)
    {
	if (this->count >= this->capacity) {
	    this->capacity *= 2;
	    this->buffer = (T*)std::realloc (this->buffer, sizeof(T)*this->capacity);
	}
	this->buffer[this->count ++] = std::move (item);
    }
    
    T& get (size_t n)
    {
	return this->buffer[n];
    }

    T *get_ptr (size_t n)
    {
	return &this->buffer[n];
    }

    T *begin () { return this->buffer; }
    T *end ()   { return &this->buffer[this->count]; }
    
    size_t len () { return this->count; }
    size_t cap () { return this->capacity; }
};
