#ifndef _TUFF_NODE_KIND_H_
#define _TUFF_NODE_KIND_H_

typedef enum {
    // Root / module-level constructs
    NODE_Module,
    NODE_Import,
    NODE_Use,
    NODE_Func,
    NODE_Struct,
    NODE_Union,
    NODE_Enum,
    NODE_Impl,
    NODE_TypeAlias,
    NODE_Trait,

    // Declarations / bindings
    NODE_Declare,
    NODE_Assign,
    NODE_Param,

    // Statements (which are also expressions in this model)
    NODE_Block,
    NODE_If,
    NODE_Else,
    NODE_While,
    NODE_For,
    NODE_Match,
    NODE_Return,
    NODE_Break,
    NODE_Continue,
    NODE_Defer,

    // Expressions
    NODE_Call,
    NODE_Index,
    NODE_FieldAccess,
    NODE_Unary,
    NODE_Binary,
    NODE_Cast,
    NODE_Group,
    NODE_Literal,
    NODE_Ident,
    NODE_Int,
    NODE_Lambda,
    NODE_Type,
    
    // Error recovery / placeholder
    NODE_Error
} NodeKind;

typedef struct Node Node;
typedef Node* NodePtr;
#endif // _TUFF_NODE_KIND_H_
