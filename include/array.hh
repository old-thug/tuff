#pragma once

#include <cstddef>
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
	this->buffer = new T[INIT_CAP];
	this->capacity = INIT_CAP;
	this->count = 0;
    }

    Array (Array& other)
	: capacity(other.capacity), count(other.count) {
	this->buffer = new T[this->capacity];
	for (size_t n = 0; n < this->count; n++) {
	    this->buffer[n] = other.buffer[n];
	}
    }

    Array (Array&& other) noexcept
	: capacity(other.capacity), count(other.count) {
	this->buffer = other.buffer;
	other.buffer = nullptr;
    }

    Array&
    operator=(Array& other) {
	if (this != &other) {
	    delete [] this->buffer;
	    this->capacity = other.capacity;
	    this->count = other.count;
	    this->buffer = new T[this->capacity];
	    for (size_t n = 0; n < this->count; n++) {
		this->buffer[n] = other.buffer[n];
	    }
	}
	return *this;
    }

    Array&
    operator=(Array&& other) noexcept {
	if (this != &other) {
	    delete [] this->buffer;
	    this->capacity = other.capacity;
	    this->count = other.count;
	    this->buffer = other.buffer;
	    other.buffer = nullptr;
	}
	return *this;
    }
    
    void
    push (T&& item)
    {
	if (this->count >= this->capacity) {
	    this->capacity *= 2;
	    auto buf = new T[this->capacity];
	    for (size_t n = 0; n < this->count; n++) {
		buf[n] = std::move (this->buffer[n]);
	    }
	    delete[] this->buffer;
	    this->buffer = buf;
	}
	this->buffer[this->count ++] = std::move (item);
    }

    void push (T& item)
    {
	if (this->count >= this->capacity) {
	    this->capacity *= 2;
	    auto buf = new T[this->capacity];
	    for (size_t n = 0; n < this->count; n++) {
		buf[n] = std::move (this->buffer[n]);
	    }
	    delete[] this->buffer;
	    this->buffer = buf;
	}
	this->buffer[this->count ++] = item;
    }
    
    T& get (size_t n)
    {
	return this->buffer[n];
    }

    T *get_ptr (size_t n)
    {
	return &this->buffer[n];
    }

    T
    operator[](size_t n) {
	return this->buffer[n];
    }
    
    T *begin () { return this->buffer; }
    T *end ()   { return &this->buffer[this->count]; }
    
    size_t len () { return this->count; }
    size_t cap () { return this->capacity; }

    ~Array () {
	delete [] this->buffer;
    }
};
