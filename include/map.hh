#pragma once

#include "array.hh"
#include <cstddef>
#include <cstring>

template<typename T> struct Hash {
    size_t operator()(const T& key);
};

template<>
struct Hash<const char *> {
    size_t operator()(const char *key) {
	size_t hash = 2166136261;
	while (key) {
	    hash ^= *key;
	    hash *= 16777219;
	}
	return hash;
    }
};

template<typename Key, typename Value> struct Bucket {
	Key key;
	Value value;
	Bucket *next;
};


template<typename Key, typename Value> struct Map {

    Array<Bucket<Key, Value>*> buckets;
    size_t size;

    Map (): buckets(), size(0) {}

    size_t
    hash (const Key& key) {
	return Hash<Key>{}();
    }
    
    void
    put (const Key& key, const Value& value) {
	size_t index = hash (key);
	auto *buck = this->buckets[index];

	while (buck) {
	    if (buck->key == key) {
		buck->value = value;
		return;
	    }
	    buck = buck->next;
	}

	buck = new Bucket<Key, Value>();
	this->buckets[index] = buck;
    }

    Value
    get (const Key& key) {
	size_t index = hash (key);
	auto *buck = this->buckets[index];

	while (buck) {
	    if (buck->key == key) {
		return buck->value;
	    }
	    buck = buck->next;
	}
	
	return Value();
    }

    bool
    contains (const Key& key) {
	size_t index = hash (key);
	auto *buck = this->buckets[index];

	while (buck) {
	    if (buck->key == key) {
		return true;
	    }
	    buck = buck->next;
	}
	
	return false;
    }
};

template <typename T> struct StringMap {
private:
    Map<const char *, T> _map;
public:
    StringMap ()
	: _map () {
    }

    Map<const char *, T> *
    operator->() {
	return &this->_map;
    }
    
    void
    put (const char *key, const T& value) {
	this->_map.put (key, value);
    }

    T
    get (const char* key) {
	size_t index = this->_map.hash (key);
	auto *buck = this->_map.buckets[index];

	while (buck) {
	    if (buck->key == key) {
		return buck->value;
	    }
	    buck = buck->next;
	}
	
	return T();
    }

    bool
    contains (const char* key) {
	size_t index = this->_map.hash (key);
	auto *buck = this->_map.buckets[index];

	while (buck) {
	    if (!strcmp(buck->key, key)) {
		return true;
	    }
	    buck = buck->next;
	}
	
	return false;
    }
};
