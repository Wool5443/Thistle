#include "Front.h"
#include "Lexer.h"
#include "RecursiveDescent.h"
#include "Common.h"

void run_front(String source_file, Str output_path)
{
    delete_comments(source_file);
    Tokens tokens = tokenize(str_ctor_string(source_file));

    Node* ast = build_ast(tokens);
    tree_draw(ast, fopen("/home/twenty/Programming/Thistle/examples/ast.dot", "w"));

    String tree = tree_write(ast);
    FILE* out = fopen(output_path.data, "w");
    if (!out)
    {
        THROW(ERROR_BAD_FILE);
    }
    if (fwrite(tree.data, 1, tree.size, out) != tree.size)
    {
        UNUSED int err = errno;
        fclose(out);
        THROW(ERROR_LINUX, "Failed to read file: %s", strerror(err));
    }
    fclose(out);
}
