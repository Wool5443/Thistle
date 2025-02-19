#include "Front.h"
#include "Lexer.h"
#include "FrontCommon.h"
#include "RecursiveDescent.h"

void run_front(Str source_file)
{
    Tokens tokens = tokenize(source_file);
    front_arena_flush();
    for (size_t i = 0, end = vec_size(tokens); i < end; i++)
    {
        printf("%d\n", tokens[i].type);
    }

    Node* ast = build_ast(tokens);
    node_print(ast, stdout);
}
