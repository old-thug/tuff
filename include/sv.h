#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define LIT(sv) (int)sv.len, sv.data
#define sv_inline static inline

typedef struct {
    const char *data;
    size_t      len;
} StringView;

sv_inline StringView
sv_init (const char *data, int len) {
    StringView sv;
    sv.data = data;
    sv.len = len;
    return sv;
}

sv_inline StringView
sv_substr (StringView sv, int start, int len) {
    return sv_init (sv.data + start, len);
}

sv_inline bool
sv_eq (StringView sv, StringView ot) {
    if (sv.len != ot.len) return false;
    return !strncmp (sv.data, ot.data, sv.len);
}
