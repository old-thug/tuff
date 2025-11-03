#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define crash() *(int*)0 = 0; abort()
#else
#define crash() exit(1)
#endif

#define internal static
#define inlined   static inline

#define NOTE(name, ...)
#define TODO(name, ...)

#define todo(fmt, ...) do { \
	fprintf (stderr, "%s:%d: unimplemented: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	crash ();							\
    } while (0)

#define assert(expr, fmt, ...) do {					\
	if (!(expr)) {							\
	    fprintf (stderr, "%s:%d: assertion failed: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	    crash ();							\
	}								\
    } while (0)

#define printn(fmt, ...) printf (fmt , ##__VA_ARGS__)
#define eprintn(fmt, ...) fprintf (stderr, fmt "\n", ##__VA_ARGS__)
#define print(fmt, ...) fprintf (stdout, fmt , ##__VA_ARGS__)
#define println(fmt, ...) fprintf (stdout, fmt "\n", ##__VA_ARGS__)
#define eprint(fmt, ...) fprintf (stderr, fmt , ##__VA_ARGS__)
#define eprintln(fmt, ...) fprintf (stderr, fmt "\n", ##__VA_ARGS__)
