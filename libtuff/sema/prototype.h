#ifndef _TUFF_SEMA_PROTOTYPE_H_
#define _TUFF_SEMA_PROTOTYPE_H_

#include "ast/type.h"
#include "array.h"

typedef arr_of (TypePtr) TypeList;
typedef struct {
    TypePtr  return_type;
    TypeList parameter_types;
} FunctionPrototype;

#endif // _TUFF_SEMA_PROTOTYPE_H_
