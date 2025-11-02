#pragma once

#include <stdio.h>
#include <stdlib.h>


#define internal static
#define inlined   static inline

#define NOTE(name, ...)
#define TODO(name, ...)

#define todo(fmt, ...) do { \
	fprintf (stderr, "error: %s:%d: unimplemented: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	exit (1); \
    } while (0)

#define printn(fmt, ...) printf (fmt , ##__VA_ARGS__); fflush (stdout)
#define eprintn(fmt, ...) fprintf (stderr, fmt "\n", ##__VA_ARGS__); fflush (stdout)
#define print(fmt, ...) fprintf (stdout, fmt , ##__VA_ARGS__); fflush (stdout)
#define println(fmt, ...) fprintf (stdout, fmt "\n", ##__VA_ARGS__); fflush (stdout)
#define eprint(fmt, ...) printf (stderr, fmt , ##__VA_ARGS__); fflush (stdout)
#define eprintln(fmt, ...) printf (stderr, fmt "\n", ##__VA_ARGS__); fflush (stdout)
