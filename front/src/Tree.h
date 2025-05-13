#ifndef THISTLE_TREE_H_
#define THISTLE_TREE_H_

#include "String.h"
#include "FrontCommon.h"

typedef enum Node_type
{
    NODE_AST,
    NODE_G,
    NODE_FUNCTION,
    NODE_FUNCTION_BODY,
    NODE_FUNCTION_SIGNATURE,
    NODE_FUNCTION_SIGNATURE_ARGS,
    NODE_IF,
    NODE_IF_BODY,
    NODE_WHILE,
    NODE_WHILE_BODY,
    NODE_BODY,
    NODE_STATEMENT,
    NODE_EXPRESSION,
    NODE_MATH_EXPRESSION,
    NODE_MATH_OPERATION,
    NODE_ASSIGN_EXPRESSION,
    NODE_RETURN_STATEMENT,
    NODE_FUNCTION_CALL,
    NODE_FUNCTION_CALL_ARGS,
    NODE_IDENTIFIER,
    NODE_NAME_TYPE,
    NODE_COMMA,
    NODE_NAME,
    NODE_STRING,
    NODE_NUMBER,
} Node_type;

typedef enum Math_operation
{
    M_PLUS,
    M_MINUS,
    M_MULTIPLY,
    M_DIVIDE,
    M_EXPONENT,

    M_ASSIGN,

    M_NOT,
    M_AND,
    M_OR,
    M_XOR,
    M_EQUAL,
    M_NOT_EQUAL,
    M_GREATER,
    M_GREATER_EQUAL,
    M_LESS,
    M_LESS_EQUAL,
} Math_operation;

typedef struct Node_data
{
    union
    {
        int    integer;
        String string;
        Math_operation operation;
    };
    Node_type type;
} Node_data;

typedef struct Node
{
    Node_data data;
    struct Node* left;
    struct Node* right;
} Node;

INLINE Str node_type_to_str(Node_type type);
INLINE String node_data_to_string(Node_data data);
INLINE Node* node_ctor(Node_data data, Node* left, Node* right);
INLINE Node* node_copy(const Node* node);
INLINE void node_print(const Node* node, FILE* out);
void tree_draw(const Node* node, FILE* out);

Node* tree_read(Str text);
String tree_write(Node* tree);

INLINE Str node_type_to_str(Node_type type)
{
    switch (type)
    {
        case NODE_AST:
            return STR_LITERAL("NODE_AST");
        case NODE_G:
            return STR_LITERAL("NODE_G");
        case NODE_FUNCTION:
            return STR_LITERAL("NODE_FUNCTION");
        case NODE_FUNCTION_BODY:
            return STR_LITERAL("NODE_FUNCTION_BODY");
        case NODE_FUNCTION_SIGNATURE:
            return STR_LITERAL("NODE_FUNCTION_SIGNATURE");
        case NODE_FUNCTION_SIGNATURE_ARGS:
            return STR_LITERAL("NODE_FUNCTION_SIGNATURE_ARGS");
        case NODE_STATEMENT:
            return STR_LITERAL("NODE_STATEMENT");
        case NODE_EXPRESSION:
            return STR_LITERAL("NODE_EXPRESSION");
        case NODE_MATH_EXPRESSION:
            return STR_LITERAL("NODE_MATH_EXPRESSION");
        case NODE_MATH_OPERATION:
            return STR_LITERAL("NODE_MATH_OPERATION");
        case NODE_ASSIGN_EXPRESSION:
            return STR_LITERAL("NODE_ASSIGN_EXPRESSION");
        case NODE_RETURN_STATEMENT:
            return STR_LITERAL("NODE_RETURN_STATEMENT");
        case NODE_FUNCTION_CALL:
            return STR_LITERAL("NODE_FUNCTION_CALL");
        case NODE_FUNCTION_CALL_ARGS:
            return STR_LITERAL("NODE_FUNCTION_CALL_ARGS");
        case NODE_IDENTIFIER:
            return STR_LITERAL("NODE_IDENTIFIER");
        case NODE_NAME_TYPE:
            return STR_LITERAL("NODE_NAME_TYPE");
        case NODE_COMMA:
            return STR_LITERAL("NODE_COMMA");
        case NODE_NAME:
            return STR_LITERAL("NODE_NAME");
        case NODE_STRING:
            return STR_LITERAL("NODE_STRING");
        case NODE_NUMBER:
            return STR_LITERAL("NODE_NUMBER");
        case NODE_IF:
            return STR_LITERAL("NODE_IF");
        case NODE_IF_BODY:
            return STR_LITERAL("NODE_IF_BODY");
        case NODE_WHILE:
            return STR_LITERAL("NODE_WHILE");
        case NODE_WHILE_BODY:
            return STR_LITERAL("NODE_WHILE_BODY");
        case NODE_BODY:
            return STR_LITERAL("NODE_BODY");
        default:
            return STR_LITERAL("UNKNOW NODE TYPE");
    }
}

INLINE String node_data_to_string(Node_data data)
{
    switch (data.type)
    {
        case NODE_STRING:
        case NODE_NAME:
            return data.string;
        case NODE_NUMBER:
            return TRY_RES(string_ctor_printf("%d", data.integer));
        case NODE_MATH_OPERATION:
            switch (data.operation)
            {
                case M_PLUS:
                    return TRY_RES(string_ctor_printf("+"));
                case M_MINUS:
                    return TRY_RES(string_ctor_printf("-"));
                case M_MULTIPLY:
                    return TRY_RES(string_ctor_printf("*"));
                case M_DIVIDE:
                    return TRY_RES(string_ctor_printf("/"));
                case M_EXPONENT:
                    return TRY_RES(string_ctor_printf("^"));
                case M_ASSIGN:
                    return TRY_RES(string_ctor_printf("="));
                case M_NOT:
                    return TRY_RES(string_ctor_printf("!"));
                case M_AND:
                    return TRY_RES(string_ctor_printf("and"));
                case M_OR:
                    return TRY_RES(string_ctor_printf("or"));
                case M_XOR:
                    return TRY_RES(string_ctor_printf("xor"));
                case M_EQUAL:
                    return TRY_RES(string_ctor_printf("=="));
                case M_NOT_EQUAL:
                    return TRY_RES(string_ctor_printf("!="));
                case M_GREATER:
                    return TRY_RES(string_ctor_printf("more"));
                case M_GREATER_EQUAL:
                    return TRY_RES(string_ctor_printf("more equal"));
                case M_LESS:
                    return TRY_RES(string_ctor_printf("less"));
                case M_LESS_EQUAL:
                    return TRY_RES(string_ctor_printf("less equal"));
                default:
                    return TRY_RES(string_ctor("UNKNONW OPERATION"));
            }
        default:
            return TRY_RES(string_ctor_printf("%s", node_type_to_str(data.type).data));
    }
}

INLINE Node* node_ctor(Node_data data, Node* left, Node* right)
{
    Node* n = front_arena_allocator.allocate(sizeof(*n));
    if (!n) THROW(ERROR_NO_MEMORY);

    *n = (Node) {
        .data = data,
        .left = left,
        .right = right,

    };

    return n;
}

INLINE Node* node_copy(const Node* node)
{
    return node_ctor(
        node->data,
        node->left ? node_copy(node->left)   : NULL,
        node->right ? node_copy(node->right) : NULL
    );
}

INLINE void node_print(const Node* node, FILE* out)
{
    if (!node || !out) THROW(ERROR_NULLPTR);

    fprintf(out, "(");
    fprintf(out, "%s", node_data_to_string(node->data).data);
    if (node->left) node_print(node->left, out);
    if (node->right) node_print(node->right, out);
    fprintf(out, ")");
}

#endif // THISTLE_TREE_H_
