#include "RecursiveDescent.h"
#include "DSL.h"

#define S if (!tokens) THROW(ERROR_NULLPTR)

#define TYPE (**tokens).type
#define NEXT ++*tokens

// x = 56 + 45 ^ 2
// 56 + 45 ^ 2

// 56 + (23 + 5 * 8) ^ (45 - 3) ^ 3
// x

// G        -> S '\0'
// S        -> {E | Symbol = E} '\0'
// E        -> T {['+', '-']T}*
// T        -> D {['*', '/']D}*
// D        -> P {'^'D}*
// P        -> -P | '(' E ')' | Id
// Symbol   -> Name
// Name     -> ALPHABET+ {DIGITS u ALPHABET}*
// Id       -> Symbol | N
// N        -> DIGITS+

static Node* get_g(Tokens* tokens);
static Node* get_s(Tokens* tokens);
static Node* get_e(Tokens* tokens);
static Node* get_t(Tokens* tokens);
static Node* get_d(Tokens* tokens);
static Node* get_p(Tokens* tokens);
static Node* get_n(Tokens* tokens);
static Node* get_id(Tokens* tokens);

Node* build_ast(Tokens tokens)
{
    S;

    Node* g = get_e(&tokens);
    return g;
}

static Node* get_g(Tokens* tokens)
{
    S;

    Node* s = get_s(tokens);

    if (TYPE != TOK_END) THROW(ERROR_SYNTAX);

    return s;
}

// S -> {E | Symbol = E} '\0'
static Node* get_s(Tokens* tokens)
{

}

// E -> T {['+', '-']T}*
static Node* get_e(Tokens* tokens)
{
    S;

    Node* p = get_t(tokens);
    Node* result = p;

    bool found_op = false;
    do
    {
        found_op = false;
        Token_type type = TYPE;
        if (type == TOK_PLUS || type == TOK_MINUS)
        {
            NEXT;
            found_op = true;
            Node* d = get_t(tokens);

            result = node_ctor(T(type), p, d);
        }
    } while (found_op);

    return result;
}

// T -> D {['*', '/']D}*
static Node* get_t(Tokens* tokens)
{
    S;

    Node* p = get_d(tokens);
    Node* result = p;

    bool found_op = false;
    do
    {
        found_op = false;
        Token_type type = TYPE;
        if (type == TOK_MULTIPLY || type == TOK_DIVIDE)
        {
            NEXT;
            found_op = true;
            Node* d = get_d(tokens);

            result = node_ctor(T(type), p, d);
        }
    } while (found_op);

    return result;
}

// D -> P {'^'D}*
static Node* get_d(Tokens* tokens)
{
    S;

    Node* p = get_p(tokens);
    Node* result = p;

    bool found_op = false;
    do
    {
        found_op = false;
        if (TYPE == TOK_POWER)
        {
            NEXT;
            found_op = true;
            Node* d = get_d(tokens);

            result = node_ctor(T(TOK_POWER), p, d);
        }
    } while (found_op);

    return result;
}

// P -> -P | '(' E ')' | Id
static Node* get_p(Tokens* tokens)
{
    S;

    if (TYPE == TOK_MINUS)
    {
        NEXT;
        Node* p = get_p(tokens);
        Node* n = node_ctor((Token) { .type = TOK_MINUS }, p, NULL);
        return n;
    }
    else if (TYPE == TOK_OPEN_BRACKET)
    {
        NEXT;
        Node* e = get_e(tokens);
        if (TYPE != TOK_CLOSE_BRACKET) THROW(ERROR_SYNTAX);
        NEXT;
        return e;
    }
    return get_id(tokens);
}

// N -> DIGITS+
static Node* get_n(Tokens* tokens)
{
    S;
    Node* n = node_ctor(**tokens, NULL, NULL);
    ++*tokens;
    return n;
}

// Id -> Name | N
static Node* get_id(Tokens* tokens)
{
    S;
    Node* n = node_ctor(**tokens, NULL, NULL);
    NEXT;
    return n;
}
