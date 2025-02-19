#ifndef THISTLE_TREE_H_
#define THISTLE_TREE_H_

#include "Tokens.h"
#include "FrontCommon.h"

typedef struct Node
{
    Token token;
    struct Node* left;
    struct Node* right;
} Node;

INLINE Node* node_ctor(Token token, Node* left, Node* right)
{
    Node* n = front_arena_allocator.allocate(sizeof(*n));
    if (!n) THROW(ERROR_NO_MEMORY);

    *n = (Node) {
        .token = token,
        .left = left,
        .right = right,
    };

    return n;
}

INLINE Node* node_copy(const Node* node)
{
    if (!node->left && !node->right)
    {
        return node_ctor(node->token, NULL, NULL);
    }
    return node_ctor(
        node->token,
        node->left ? node_copy(node->left)   : NULL,
        node->right ? node_copy(node->right) : NULL
    );
}

INLINE void node_print(const Node* node, FILE* out)
{
    if (!node || !out) THROW(ERROR_NULLPTR);

    fprintf(out, "(");
    token_print(node->token, out);
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
    token_print(node->token, out);
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
