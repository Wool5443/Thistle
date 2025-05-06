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
        fprintf(stdout, "%s", token_to_string(tokens[i]).data);
        printf(" ");
    }
    printf("\n");
    Node* ast = build_ast(tokens);
    node_draw(ast, fopen("/home/twenty/Programming/Thistle/examples/ast.dot", "w"));
}
