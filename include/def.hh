#pragma once

#include <cstdio>
#include <cstdlib>


#define NOTE(name, ...)
#define TODO(name, ...)

#define todo(fmt, ...) do { \
	fprintf (stderr, "error: %s:%d: unimplemented: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	exit (1); \
    } while (0)

#define printn(fmt, ...) printf (fmt , ##__VA_ARGS__); fflush (stdout)
