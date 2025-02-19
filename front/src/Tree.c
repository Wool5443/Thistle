#include "Tree.h"

Node* node_ctor(Token t, Node* left, Node* right);
Node* node_copy(const Node* restrict node);
void node_print(const Node* node, FILE* out);
