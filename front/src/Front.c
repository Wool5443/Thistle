#include "Front.h"
#include "Lexer.h"
#include "RecursiveDescent.h"

void run_front(Str source_file)
{
    Tokens tokens = tokenize(source_file);
    VEC_ITER(tokens, i)
    {
        fprintf(stdout, "%s", token_to_string(tokens[i]).data);
        printf(" ");
    }
    printf("\n");
    Node* ast = build_ast(tokens);
    node_draw(ast, fopen("/home/twenty/Programming/Thistle/examples/ast.dot", "w"));
}
