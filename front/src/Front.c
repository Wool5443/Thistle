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
        token_print(tokens[i], stdout);
        printf(" ");
    }
    printf("\n");
    Node* ast = build_ast(tokens);
    node_print(ast, stdout);
    printf("\n");
}
