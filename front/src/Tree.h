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

INLINE Node* node_ctor(Token t, Node* left, Node* right)
{
    Node* n = front_arena_allocator.allocate(sizeof(*n));
    if (!n) THROW(ERROR_NO_MEMORY);

    *n = (Node) {
        .token = t,
        .left = left,
        .right = right,
    };

    return n;
}

#endif // THISTLE_TREE_H_
