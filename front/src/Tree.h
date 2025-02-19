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

INLINE Node* node_copy(const Node* restrict node)
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
    if (node->left) node_print(node->left, out);
    token_print(node->token, out);
    if (node->right) node_print(node->right, out);
    fprintf(out, ")");
}

#endif // THISTLE_TREE_H_
