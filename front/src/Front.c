#include "Front.h"

#include "Common.h"
#include "Lexer.h"
#include "RecursiveDescent.h"

void run_front(const char* input_path, const char* output_path)
{
    String source_file = TRY_RES(read_file(input_path));

    delete_comments(source_file);
    Tokens tokens = tokenize(str_ctor_string(source_file));

    VEC_ITER(tokens, i)
    {
        printf("%s ", token_to_string(tokens[i]).data);
    }
    putchar('\n');

    Node* ast = build_ast(tokens);
    tree_draw(ast, fopen("ast.dot", "w"));

    String tree = tree_write(ast);
    FILE* out = fopen(output_path, "w");
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
