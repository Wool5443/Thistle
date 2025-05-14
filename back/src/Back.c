#include "Back.h"
#include "Common.h"
#include "Tree.h"
#include "Symbols.h"

static void run_node(Node* node);

void run_back(const char* input_path, const char* output_path)
{
    String tree_string = TRY_RES(read_file(input_path));
    Node* tree = tree_read(str_ctor_string(tree_string));

    Symbol_table symbols = {};

    symbol_add(&symbols, TRY_RES(string_ctor("print")));

    VEC_ITER(symbols, i)
    {
        printf("%s\n", symbols[i].name.data);
    }
}

static void run_node(Node* node)
{

}
