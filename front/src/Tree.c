#include "Tree.h"

Str node_type_to_str(Node_type type);
String node_data_to_string(Node_data data);
Node* node_ctor(Node_data t, Node* left, Node* right);
Node* node_copy(const Node* node);
void node_print(const Node* node, FILE* out);
void node_draw(const Node* node, FILE* out);
void node_build_graph_rec_(const Node* node, FILE* out);
void node_draw_graph_rec_(const Node* node, FILE* out);
