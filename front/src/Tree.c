#include "Tree.h"

Node* node_ctor(Token t, Node* left, Node* right);
Node* node_copy(const Node* node);
void node_print(const Node* node, FILE* out);
void node_draw(const Node* node, FILE* out);
void node_build_graph_rec_(const Node* node, FILE* out);
void node_draw_graph_rec_(const Node* node, FILE* out);
