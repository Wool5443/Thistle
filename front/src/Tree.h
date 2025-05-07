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
    NODE_STATEMENT,
    NODE_EXPRESSION,
    NODE_MATH_EXPRESSION,
    NODE_MATH_OPERATION,
    NODE_ASSIGN_EXPRESSION,
    NODE_RETURN_STATEMENT,
    NODE_PRINT_EXPRESSION,
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

    M_NOT,
    M_AND,
    M_OR,
    M_XOR,
    M_EQUALS,
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
        case NODE_PRINT_EXPRESSION:
            return STR_LITERAL("NODE_PRINT_EXPRESSION");
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
            return TRY_RES(string_printf("%d", data.integer));
        case NODE_MATH_OPERATION:
            switch (data.operation)
            {
                case M_PLUS:
                    return TRY_RES(string_printf("+"));
                case M_MINUS:
                    return TRY_RES(string_printf("-"));
                case M_MULTIPLY:
                    return TRY_RES(string_printf("*"));
                case M_DIVIDE:
                    return TRY_RES(string_printf("/"));
                case M_EXPONENT:
                    return TRY_RES(string_printf("^"));
                default:
                    return TRY_RES(string_ctor("UNKNONW OPERATION"));
            }
        default:
            return TRY_RES(string_printf("%s", node_type_to_str(data.type).data));
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

INLINE void node_build_graph_rec_(const Node* node, FILE* out);
INLINE void node_draw_graph_rec_(const Node* node, FILE* out);

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define TREE_COLOR "\"#ff7be9\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""

INLINE void node_draw(const Node* node, FILE* out)
{
    assert(node && out);

    fprintf(out,
        "digraph\n"
        "{\n"
        "rankdir = TB;\n"
        "node[shape = record, color = " NODE_FRAME_COLOR ", fontname = " FONT_NAME ", fontsize = " FONT_SIZE "];\n"
        "bgcolor = " BACK_GROUND_COLOR ";\n"
    );

    node_build_graph_rec_(node, out);
    node_draw_graph_rec_(node, out);

    fprintf(out, "\n");
    fprintf(out, "NODE_%p\n", node);

    fprintf(out, "}\n");
}

INLINE void node_build_graph_rec_(const Node* node, FILE* out)
{
    assert(node && out);

    fprintf(out, "NODE_%p[style = \"filled\", fillcolor = \"#fae1f6\", ", node);
    fprintf(out, "label = \"{Value:\\n");
    fprintf(out, "%s", node_data_to_string(node->data).data);
    fprintf(out, "|{<left>left|<right>right}}\"];\n");

    if (node->left) node_build_graph_rec_(node->left, out);
    if (node->right) node_build_graph_rec_(node->right, out);
}

INLINE void node_draw_graph_rec_(const Node* node, FILE* out)
{
    if (node->left)
    {
        fprintf(out, "NODE_%p:left->NODE_%p;\n", node, node->left);
        node_draw_graph_rec_(node->left, out);
    }
    if (node->right)
    {
        fprintf(out, "NODE_%p:right->NODE_%p;\n", node, node->right);
        node_draw_graph_rec_(node->right, out);
    }
}

#undef FONT_SIZE
#undef FONT_NAME
#undef BACK_GROUND_COLOR
#undef NODE_COLOR
#undef NODE_FRAME_COLOR
#undef ROOT_COLOR
#undef FREE_HEAD_COLOR

#endif // THISTLE_TREE_H_
