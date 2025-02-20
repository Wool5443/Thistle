#include "RecursiveDescent.h"
#include "DSL.h"

#define S assert(tokens)

#define CHECK_TOK(t) if (TYPE != t) THROW(ERROR_SYNTAX)

#define TYPE (**tokens).type
#define NEXT ++*tokens

// x = 56 + 45 ^ 2
// 56 + 45 ^ 2

// 56 + (23 + 5 * 8) ^ (45 - 3) ^ 3
// x

// G        -> S+ TOK_END
// S        -> {{ E | ASSIGN ;} | IF | BLOCK}
// IF       -> if E S {else S}?
// FOR      -> for '(' {E | ASSIGN}; E; S? ')'
// ASSIGN   -> let name = E
// BLOCK    -> '{' S+ '}'
// E        -> T {['+', '-']T}*
// T        -> D {['*', '/']D}*
// D        -> P {'^'D}*
// P        -> -P | '(' E ')' | Id
// Name     -> ALPHABET+ {DIGITS u ALPHABET}*
// Id       -> Symbol | N
// N        -> DIGITS+

static Node* get_g(Tokens* tokens);
static Node* get_block(Tokens* tokens);
static Node* get_s(Tokens* tokens);
static Node* get_assign(Tokens* tokens);
static Node* get_for(Tokens* tokens);
static Node* get_if(Tokens* tokens);
static Node* get_e(Tokens* tokens);
static Node* get_t(Tokens* tokens);
static Node* get_d(Tokens* tokens);
static Node* get_p(Tokens* tokens);
static Node* get_n(Tokens* tokens);
static Node* get_id(Tokens* tokens);

Node* build_ast(Tokens tokens)
{
    S;

    Node* g = get_g(&tokens);
    return g;
}

static Node* get_g(Tokens* tokens)
{
    S;

    Node* s = get_s(tokens);

    while (TYPE != TOK_END)
    {
        Node* next_s = get_s(tokens);
        s = node_ctor(T(TOK_SEMI_COLON), s, next_s);
    }

    return s;
}

static Node* get_block(Tokens* tokens)
{
    S;

    CHECK_TOK(TOK_OPEN_SCOPE);
    NEXT;

    Node* s = get_s(tokens);

    while (TYPE != TOK_CLOSE_SCOPE && TYPE != TOK_END)
    {
        Node* next_s = get_s(tokens);
        s = node_ctor(T(TOK_SEMI_COLON), s, next_s);
    }

    CHECK_TOK(TOK_CLOSE_SCOPE);
    NEXT;

    return node_ctor(T(TOK_BLOCK), s, NULL);
}

// S -> {{ E | ASSIGN ;} | IF | FOR | BLOCK}
static Node* get_s(Tokens* tokens)
{
    S;

    Node* result = NULL;
    switch (TYPE)
    {
        case TOK_LET:
            result = get_assign(tokens);
            CHECK_TOK(TOK_SEMI_COLON);
            NEXT;
            break;
        case TOK_FOR:
            result = get_for(tokens);
            break;
        case TOK_IF:
            result = get_if(tokens);
            break;
        case TOK_OPEN_SCOPE:
            result = get_block(tokens);
            break;
        default:
            result = get_e(tokens);
            CHECK_TOK(TOK_SEMI_COLON);
            NEXT;
            break;
    }

    return result;
}

static Node* get_assign(Tokens* tokens)
{
    S;

    CHECK_TOK(TOK_LET);
    NEXT;

    Node* name = node_ctor(**tokens, NULL, NULL);
    NEXT;
    CHECK_TOK(TOK_ASSIGNMENT);
    NEXT;
    Node* val = get_e(tokens);
    Node* assign = node_ctor(T(TOK_ASSIGNMENT), name, val);

    return assign;
}

// FOR -> for '(' E | ASSIGN; E; S? ')' S
static Node* get_for(Tokens* tokens)
{
    S;

    CHECK_TOK(TOK_FOR); NEXT;
    CHECK_TOK(TOK_OPEN_BRACKET); NEXT;

    Node* a = NULL;
    switch (TYPE)
    {
        case TOK_LET:
            a = get_assign(tokens);
            break;
        default:
            a = get_e(tokens);
            break;
    }
    CHECK_TOK(TOK_SEMI_COLON); NEXT;

    Node* b = get_e(tokens);

    CHECK_TOK(TOK_SEMI_COLON); NEXT;

    Node* c = NULL;
    switch (TYPE)
    {
        case TOK_CLOSE_BRACKET:
            break;
        default:
            c = get_s(tokens);
            break;
    }
    CHECK_TOK(TOK_CLOSE_BRACKET); NEXT;

    Node* body = get_s(tokens);

    Node* inbrackets = node_ctor(T(TOK_SEMI_COLON), a, node_ctor(T(TOK_SEMI_COLON), b, c));

    return node_ctor(T(TOK_FOR), inbrackets, body);
}

// IF -> if E S {else S}?
static Node* get_if(Tokens* tokens)
{
    S;

    CHECK_TOK(TOK_IF);
    NEXT;

    Node* e = get_e(tokens);
    Node* s = get_s(tokens);

    Node* result = node_ctor(T(TOK_IF), e, s);

    if (TYPE == TOK_ELSE)
    {
        NEXT;
        Node* ss = get_s(tokens);
        result = node_ctor(T(TOK_ELSE), result, ss);
    }

    return result;
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
    NEXT;
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
