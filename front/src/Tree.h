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

INLINE void node_data_print(Node_data data, FILE* out)
{
    switch (data.type)
    {
        case NODE_STRING:
        case NODE_NAME:
            fprintf(out, "%s", data.string.data);
            break;
        case NODE_FUNCTION_SIGNATURE:
            fprintf(out, "NODE_FUNCTION_SIGNATURE");
            break;
        case NODE_NAME_TYPE:
            fprintf(out, "NODE_NAME_TYPE");
            break;
        case NODE_FUNCTION:
            fprintf(out, "NODE_FUNCTION");
            break;
        case NODE_FUNCTION_CALL:
            fprintf(out, "NODE_FUNCTION_CALL");
            break;
        case NODE_NUMBER:
            fprintf(out, "%d", data.integer);
            break;
        case NODE_AST:
            fprintf(out, "NODE_AST");
            break;
        case NODE_G:
            fprintf(out, "NODE_G");
            break;
        case NODE_FUNCTION_SIGNATURE_ARGS:
            fprintf(out, "NODE_FUNCTION_DEFINITION_ARGS");
            break;
        case NODE_FUNCTION_CALL_ARGS:
            fprintf(out, "NODE_FUNCTION_CALL_ARGS");
            break;
        case NODE_FUNCTION_BODY:
            fprintf(out, "NODE_FUNCTION_BODY");
            break;
        case NODE_STATEMENT:
            fprintf(out, "NODE_STATEMENT");
            break;
        case NODE_EXPRESSION:
            fprintf(out, "NODE_EXPRESSION");
            break;
        case NODE_MATH_EXPRESSION:
            fprintf(out, "NODE_MATH_EXPRESSION");
            break;
        case NODE_ASSIGN_EXPRESSION:
            fprintf(out, "NODE_ASSIGN_EXPRESSION");
            break;
        case NODE_PRINT_EXPRESSION:
            fprintf(out, "NODE_PRINT_EXPRESSION");
            break;
        case NODE_RETURN_STATEMENT:
            fprintf(out, "NODE_RETURN_EXPRESSION");
            break;
        case NODE_IDENTIFIER:
            fprintf(out, "NODE_IDENTIFIER");
            break;
        case NODE_COMMA:
            fprintf(out, "NODE_COMMA");
            break;
        case NODE_MATH_OPERATION:
            switch (data.operation)
            {
                case M_PLUS:
                    fprintf(out, "+");
                    break;
                case M_MINUS:
                    fprintf(out, "-");
                    break;
                case M_MULTIPLY:
                    fprintf(out, "*");
                    break;
                case M_DIVIDE:
                    fprintf(out, "/");
                    break;
                case M_EXPONENT:
                    fprintf(out, "^");
                    break;
            }
            break;
        default:
            fprintf(out, "UNKNOWN NODE_DATA: %d", data.type);
            break;
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
    node_data_print(node->data, out);
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
    node_data_print(node->data, out);
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
