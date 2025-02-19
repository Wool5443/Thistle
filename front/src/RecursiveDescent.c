#include "RecursiveDescent.h"

static Node* get_g(Token* tokens);

Node* build_ast(Tokens tokens)
{
    Node* g = get_g(&tokens);
}

static Node* get_g(Token* tokens)
{
    if (!tokens) TH(ERROR_NULLPTR);


}
