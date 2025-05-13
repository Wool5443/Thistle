#include "Tree.h"
#include "Common.h"

static void node_write_(Node* node, String* output);
static void node_draw_graph_rec_(const Node* node, FILE* out);
static void node_build_graph_rec_(const Node* node, FILE* out);

static Node* node_read_(const char** text);

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define TREE_COLOR "\"#ff7be9\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""

Str node_type_to_str(Node_type type)
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
    case NODE_IF:
        return STR_LITERAL("NODE_IF");
    case NODE_IF_BODY:
        return STR_LITERAL("NODE_IF_BODY");
    case NODE_WHILE:
        return STR_LITERAL("NODE_WHILE");
    case NODE_WHILE_BODY:
        return STR_LITERAL("NODE_WHILE_BODY");
    case NODE_BODY:
        return STR_LITERAL("NODE_BODY");
    default:
        return STR_LITERAL("UNKNOW NODE TYPE");
    }
}

Node_type str_to_node_type(Str string)
{
    for (int i = 1; i < NODE_TYPE_COUNT; i++)
    {
        Str t = node_type_to_str((Node_type)i);
        if (str_compare(t, string) == 0)
        {
            return (Node_type)i;
        }
    }
    return NODE_BAD;
}

Str math_operation_to_str(Math_operation op)
{
    switch (op)
    {
        case M_PLUS:
            return STR_LITERAL("+");
        case M_MINUS:
            return STR_LITERAL("-");
        case M_MULTIPLY:
            return STR_LITERAL("*");
        case M_DIVIDE:
            return STR_LITERAL("/");
        case M_EXPONENT:
            return STR_LITERAL("^");
        case M_ASSIGN:
            return STR_LITERAL("=");
        case M_NOT:
            return STR_LITERAL("!");
        case M_AND:
            return STR_LITERAL("and");
        case M_OR:
            return STR_LITERAL("or");
        case M_XOR:
            return STR_LITERAL("xor");
        case M_EQUAL:
            return STR_LITERAL("==");
        case M_NOT_EQUAL:
            return STR_LITERAL("!=");
        case M_GREATER:
            return STR_LITERAL(">");
        case M_GREATER_EQUAL:
            return STR_LITERAL(">=");
        case M_LESS:
            return STR_LITERAL("<");
        case M_LESS_EQUAL:
            return STR_LITERAL("<=");
        default:
            return STR_LITERAL("UNKNOWN MATH OPERATION");
    }
}

Math_operation str_to_math_operation(Str string)
{
    for (int i = 1; i < MATH_OPERATION_COUNT; i++)
    {
        Str t = math_operation_to_str((Math_operation)i);
        if (str_compare(t, string) == 0)
        {
            return (Math_operation)i;
        }
    }
    return M_BAD_OPERATION;
}

String node_data_to_string(Node_data data)
{
    String result = TRY_RES(string_ctor_str(node_type_to_str(data.type)));
    switch (data.type)
    {
    case NODE_STRING:
    case NODE_NAME:
        TRY(string_printf(&result, "(%s)", data.string.data));
        break;
    case NODE_NUMBER:
        TRY(string_printf(&result, "(%d)", data.integer));
        break;
    case NODE_MATH_OPERATION:
        TRY(string_printf(&result, "(%s)", math_operation_to_str(data.operation).data));
        break;
    default:
        TRY(string_append(&result, "()"));
        break;
    }
    return result;
}

Node_data str_to_node_data(Str string)
{
    log_debug("str to node data: %zu\n%s", string.size, string.data);
    if (string.data[string.size - 1] != ')')
    {
        THROW(ERROR_SYNTAX, "')' expected");
    }

    const char* open = strchr(string.data + 1, '(');
    if (!open)
    {
        THROW(ERROR_SYNTAX, "'(' expected");
    }

    Str type_str = TRY_RES(str_slice(string, 0, open - string.data));
    Node_type type = str_to_node_type(type_str);

    Str data_str = TRY_RES(
            str_slice(
                string,
                open - string.data + 1,
                string.size - 1
            )
    );

    Node_data data = { .type = type };
    switch (type)
    {
        case NODE_STRING:
        case NODE_NAME:
            data.string = TRY_RES(string_ctor_str(data_str));
            break;
        case NODE_MATH_OPERATION:
            data.operation = str_to_math_operation(data_str);
            break;
        case NODE_NUMBER:
            data.integer = strtol(data_str.data, NULL, 0);
            break;
        default:
            break;
    }

    return data;
}

Node* node_ctor(Node_data data, Node* left, Node* right)
{
    Node* n = thistle_arena_allocator.allocate(sizeof(*n));
    if (!n)
    {
        THROW(ERROR_NO_MEMORY);
    }

    *n = (Node) {
        .data = data,
        .left = left,
        .right = right,

    };

    return n;
}

Node* node_copy(const Node* node)
{
    return node_ctor(node->data, node->left ? node_copy(node->left) : NULL,
                     node->right ? node_copy(node->right) : NULL);
}

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

Node* tree_read(Str text)
{
    const char* s = text.data;

    return node_read_(&s);
}

#define CURRENT (**text)
#define SKIP(n) ((*text) += n)
#define CHECK(c)                                            \
do                                                          \
{                                                           \
    if (CURRENT != c)                                       \
    {                                                       \
        THROW(ERROR_SYNTAX, #c " expected");                \
    }                                                       \
    SKIP(1);                                                \
} while (0)

static Node* node_read_(const char** text)
{
    log_debug("Reading node, *text ==\n%s", *text);
    CHECK('(');

    const char* close_data = strchr(*text, ')');
    if (!close_data)
    {
        THROW(ERROR_SYNTAX, "')' expected");
    }
    if (close_data == *text)
    {
        SKIP(1);
        return NULL;
    }

    size_t data_size = close_data - *text + 1;
    Str data_str = str_ctor_size(*text, data_size);
    Node_data data = str_to_node_data(data_str);
    SKIP(data_size);

    CHECK(';');
    Node* left = node_read_(text);
    CHECK(';');
    Node* right = node_read_(text);
    CHECK(')');

    Node* node = node_ctor(data, left, right);

    return node;
}
#undef CURRENT
#undef SKIP
#undef CHECK

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
    TRY(string_append_char(output, ';'));
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
    {
        node_build_graph_rec_(node->left, out);
    }
    if (node->right)
    {
        node_build_graph_rec_(node->right, out);
    }
}
