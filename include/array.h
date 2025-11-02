#pragma once

#include <stddef.h>
#include <stdlib.h>

#define arr_of(type) struct { type* items; size_t count; size_t capacity; }

#define arr_resize(arr, n) do {\
	(arr)->capacity = n;					\
	(arr)->items = realloc ((arr)->items, (arr)->capacity*sizeof (*(arr)->items));\
    } while (0)

#define arr_resize_if_needed(arr) do {					\
	if ((arr)->count >= (arr)->capacity) {				\
	    arr_resize (arr, (arr)->capacity == 0? 4:(arr)->capacity*2); \
	}								\
    } while (0)

#define arr_push(arr, item) do {		\
	arr_resize_if_needed (arr);		\
	(arr)->items[(arr)->count++] = item;	\
    } while (0)

#define arr_get(arr, n) (arr)->items[n]
#define arr_len(arr) (arr)->count
