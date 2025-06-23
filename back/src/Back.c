#include "Back.h"

#include "Common.h"
#include "Symbols.h"
#include "Tree.h"

static void run_node(Node* node);

void run_back(const char* input_path, UNUSED const char* output_path)
{
    String tree_string = TRY_RES(read_file(input_path));
    UNUSED Node* tree = tree_read(str_ctor_string(tree_string));

    Symbol_table symbols = {};

    symbol_add(&symbols, TRY_RES(string_ctor("print")));

    VEC_ITER(symbols, i)
    {
        printf("%s\n", symbols[i].name.data);
    }
}

UNUSED static void run_node(UNUSED Node* node)
{
}
