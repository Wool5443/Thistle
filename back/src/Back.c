#include "Back.h"
#include "Common.h"
#include "Tree.h"

void run_back(const char* input_path, const char* output_path)
{
    String tree_string = TRY_RES(read_file(input_path));
    Node* tree = tree_read(str_ctor_string(tree_string));
}
