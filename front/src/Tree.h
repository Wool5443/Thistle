#ifndef THISTLE_TREE_H_
#define THISTLE_TREE_H_

#include "String.h"

typedef enum Node_type
{
    NODE_BAD,
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

    NODE_TYPE_COUNT,
} Node_type;

typedef enum Math_operation
{
    M_BAD_OPERATION,
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

    MATH_OPERATION_COUNT,
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

Str node_type_to_str(Node_type type);
Node_type str_to_node_type(Str string);
Str math_operation_to_str(Math_operation op);
Math_operation str_to_math_operation(Str string);

String node_data_to_string(Node_data data);
Node* node_ctor(Node_data data, Node* left, Node* right);
Node* node_copy(const Node* node);

void tree_draw(const Node* node, FILE* out);
Node* tree_read(Str text);
String tree_write(Node* tree);

#endif // THISTLE_TREE_H_
