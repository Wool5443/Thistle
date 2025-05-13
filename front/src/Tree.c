#include "Tree.h"

Str node_type_to_str(Node_type type);
String node_data_to_string(Node_data data);
Node* node_ctor(Node_data data, Node* left, Node* right);
Node* node_copy(const Node* node);
void node_print(const Node* node, FILE* out);

static void node_write_(Node* node, String* output);
static void node_draw_graph_rec_(const Node* node, FILE* out);
static void node_build_graph_rec_(const Node* node, FILE* out);

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define TREE_COLOR "\"#ff7be9\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""

void tree_draw(const Node* root, FILE* out)
{
    assert(root && out);

    fprintf(out,
            "digraph\n"
            "{\n"
            "rankdir = TB;\n"
            "node[shape = record, color = " NODE_FRAME_COLOR ", fontname = " FONT_NAME ", fontsize = " FONT_SIZE "];\n"
            "bgcolor = " BACK_GROUND_COLOR ";\n");

    node_build_graph_rec_(root, out);
    node_draw_graph_rec_(root, out);

    fprintf(out, "\n");
    fprintf(out, "NODE_%p\n", root);

    fprintf(out, "}\n");
}

Node* tree_read(Str text);

String tree_write(Node* tree)
{
    assert(tree);

    String result = {};
    node_write_(tree, &result);

    return result;
}

static void node_write_(Node* node, String* output)
{
    assert(output);

    if (!node)
    {
        TRY(string_append(output, "()"));
        return;
    }

    TRY(string_printf(output, "(%s;", node_data_to_string(node->data).data));
    node_write_(node->left, output);
    node_write_(node->right, output);
    TRY(string_append_char(output, ')'));
}

static void node_draw_graph_rec_(const Node* node, FILE* out)
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

static void node_build_graph_rec_(const Node* node, FILE* out)
{
    assert(node && out);

    fprintf(out, "NODE_%p[style = \"filled\", fillcolor = \"#fae1f6\", ", node);
    fprintf(out, "label = \"{Value:\\n");
    fprintf(out, "%s", node_data_to_string(node->data).data);
    fprintf(out, "|{<left>left|<right>right}}\"];\n");

    if (node->left)
        node_build_graph_rec_(node->left, out);
    if (node->right)
        node_build_graph_rec_(node->right, out);
}
