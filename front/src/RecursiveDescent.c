#include "RecursiveDescent.h"

#include "Common.h"
#include "DSL.h"

#define S assert(tokens)

#define CHECK_TOK(t, ...)                                                      \
    if (TOKEN != t)                                                            \
    THROW(ERROR_SYNTAX __VA_OPT__(, ) __VA_ARGS__)
#define TOKEN (fe_tokens->type)
#define NEXT (++fe_tokens)
#define PREV (--fe_tokens)

#define GET_INTRO()                                                            \
    log_debug("Enter %s, current token: %s(%u)",                               \
              __PRETTY_FUNCTION__,                                             \
              token_to_string(*fe_tokens).data,                                \
              TOKEN)
#define GET_OUTRO(node)                                                        \
    do                                                                         \
    {                                                                          \
        Node* node_t = (node);                                                 \
        log_debug("Leave %s, current node: %s",                                \
                  __PRETTY_FUNCTION__,                                         \
                  node_data_to_string(node_t->data).data);                     \
        return node_t;                                                         \
    } while (0)
// #define GET_INTRO()

static Tokens fe_tokens;

static Node* get_g();
static Node* get_function();
static Node* get_function_signature();
static Node* get_function_signature_args();
static Node* get_if_while();
static Node* get_while();
static Node* get_while_body();
static Node* get_if();
static Node* get_if_body();
static Node* get_else();
static Node* get_body();
static Node* get_statement();
static Node* get_return_statement();
static Node* get_expression();
static Node* get_assign_expression();
static Node* get_function_call();
static Node* get_function_call_args();
static Node* get_math_expression();
static Node* get_X();
static Node* get_O();
static Node* get_CMP();
static Node* get_A();
static Node* get_AS();
static Node* get_E();
static Node* get_T();
static Node* get_D();
static Node* get_P();
static Node* get_identifier();
static Node* get_name_type();
static Node* get_name();
static Node* get_string();
static Node* get_number();

Node* build_ast(Tokens tokens)
{
    fe_tokens = tokens;

    return get_g();
}

static Node* get_g()
{
    GET_INTRO();

    Node* function = get_function();
    Node* G = node_ctor(N(NODE_G), function, NULL);

    while (TOKEN != TOK_END)
    {
        Node* next = get_function();
        G = node_ctor(N(NODE_G), G, next);
    }

    GET_OUTRO(G);
}

static Node* get_function()
{
    GET_INTRO();

    CHECK_TOK(TOK_FN,
              "'fn' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* function_signature = get_function_signature();

    Node* result = NULL;

    switch (TOKEN)
    {
        case TOK_SEMI_COLON:
        {
            NEXT;
            Node* function =
                node_ctor(N(NODE_FUNCTION), function_signature, NULL);
            result = function;
            break;
        }
        case TOK_OPEN_SCOPE:
        {
            Node* function_body = get_body();
            Node* function =
                node_ctor(N(NODE_FUNCTION), function_signature, function_body);
            result = function;
            break;
        }
        default:
            THROW(ERROR_SYNTAX,
                  "';' or function body expected, got %s",
                  token_to_string(*fe_tokens).data);
    }

    GET_OUTRO(result);
}

static Node* get_if_while()
{
    GET_INTRO();

    Node* result = NULL;

    switch (TOKEN)
    {
        case TOK_IF:
            result = get_if();
            break;
        case TOK_WHILE:
            result = get_while();
            break;
        default:
            THROW(ERROR_SYNTAX,
                  "'if' or 'while' expected, got %s",
                  token_to_string(*fe_tokens).data);
    }

    GET_OUTRO(result);
}

static Node* get_while()
{
    GET_INTRO();

    CHECK_TOK(TOK_WHILE,
              "'while' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* cond = get_expression();
    Node* while_body = get_while_body();
    Node* while_ = node_ctor(N(NODE_WHILE), cond, while_body);

    GET_OUTRO(while_);
}

static Node* get_while_body()
{
    GET_INTRO();
    GET_OUTRO(get_body());
}

static Node* get_if()
{
    GET_INTRO();

    CHECK_TOK(TOK_IF,
              "'if' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* cond = get_expression();
    Node* if_body = get_if_body();
    Node* if_ = node_ctor(N(NODE_IF), cond, if_body);

    GET_OUTRO(if_);
}

static Node* get_if_body()
{
    GET_INTRO();

    Node* body = get_body();

    Node* result = body;

    if (TOKEN == TOK_ELSE)
    {
        Node* els = get_else();
        Node* if_body = node_ctor(N(NODE_IF_BODY), body, els);
        result = if_body;
    }
    else
    {
        body->data.type = NODE_IF_BODY;
    }

    GET_OUTRO(result);
}

static Node* get_else()
{
    GET_INTRO();

    CHECK_TOK(TOK_ELSE,
              "'else' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* result = NULL;

    switch (TOKEN)
    {
        case TOK_IF:
            result = get_if();
            break;
        default:
            result = get_body();
            break;
    }

    GET_OUTRO(result);
}

static Node* get_body()
{
    GET_INTRO();

    CHECK_TOK(TOK_OPEN_SCOPE,
              "'{' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* statement = get_statement();

    while (TOKEN != TOK_CLOSE_SCOPE)
    {
        Node* next_statement = get_statement();
        statement = node_ctor(N(NODE_STATEMENT), statement, next_statement);
    }

    CHECK_TOK(TOK_CLOSE_SCOPE,
              "'}' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* body = node_ctor(N(NODE_BODY), statement, NULL);

    GET_OUTRO(body);
}

static Node* get_function_signature()
{
    GET_INTRO();

    Node* name = get_name();

    CHECK_TOK(TOK_OPEN_BRACKET,
              "'(' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* function_signature_args = NULL;

    if (TOKEN != TOK_CLOSE_BRACKET)
    {
        function_signature_args = get_function_signature_args();
    }

    CHECK_TOK(TOK_CLOSE_BRACKET,
              "')' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* function_signature =
        node_ctor(N(NODE_FUNCTION_SIGNATURE), name, function_signature_args);

    GET_OUTRO(function_signature);
}

static Node* get_function_signature_args()
{
    GET_INTRO();

    Node* function_sig_args = get_name_type();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        CHECK_TOK(TOK_COMMA,
                  "',' expected, got %s",
                  token_to_string(*fe_tokens).data);
        NEXT;
        Node* name_type = get_name_type();
        function_sig_args =
            node_ctor(N(NODE_COMMA), function_sig_args, name_type);
    }

    GET_OUTRO(function_sig_args);
}

static Node* get_statement()
{
    GET_INTRO();

    Node* result = NULL;

    switch (TOKEN)
    {
        case TOK_RETURN:
            Node* return_ = get_return_statement();
            CHECK_TOK(TOK_SEMI_COLON,
                      "';' expected, got %s",
                      token_to_string(*fe_tokens).data);
            NEXT;
            result = return_;
            break;
        case TOK_IF:
        case TOK_WHILE:
            result = get_if_while();
            break;
        default:
        {
            Node* expression = get_expression();
            CHECK_TOK(TOK_SEMI_COLON,
                      "';' expected, got %s",
                      token_to_string(*fe_tokens).data);
            NEXT;
            result = expression;
            break;
        }
    }

    GET_OUTRO(result);
}

static Node* get_expression()
{
    GET_INTRO();

    Node* expression = NULL;

    switch (TOKEN)
    {
        case TOK_LET:
            expression = get_assign_expression();
            break;
        case TOK_NAME:
            NEXT;
            if (TOKEN == TOK_OPEN_BRACKET)
            {
                PREV;
                expression = get_function_call();
            }
            else
            {
                PREV;
                expression = get_math_expression();
            }
            break;
        case TOK_STRING:
            expression = get_string();
            break;
        default:
            expression = get_math_expression();
            break;
    }

    GET_OUTRO(expression);
}

static Node* get_function_call()
{
    GET_INTRO();

    Node* name = get_name();
    CHECK_TOK(TOK_OPEN_BRACKET,
              "'(' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* args = NULL;
    if (TOKEN != TOK_CLOSE_BRACKET)
    {
        args = get_function_call_args();
    }

    CHECK_TOK(TOK_CLOSE_BRACKET,
              "')' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* function_call = node_ctor(N(NODE_FUNCTION_CALL), name, args);

    GET_OUTRO(function_call);
}

static Node* get_function_call_args()
{
    GET_INTRO();

    Node* function_call_args = get_expression();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        CHECK_TOK(TOK_COMMA,
                  "',' expected, got %s",
                  token_to_string(*fe_tokens).data);
        NEXT;
        Node* expression = get_expression();
        function_call_args =
            node_ctor(N(NODE_COMMA), function_call_args, expression);
    }

    GET_OUTRO(function_call_args);
}

static Node* get_return_statement()
{
    GET_INTRO();

    CHECK_TOK(TOK_RETURN,
              "'return' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* return_statement = NULL;

    if (TOKEN == TOK_SEMI_COLON)
    {
        NEXT;
        return node_ctor(N(NODE_RETURN_STATEMENT), NULL, NULL);
    }

    Node* expression = get_expression();

    return_statement = node_ctor(N(NODE_RETURN_STATEMENT), expression, NULL);

    GET_OUTRO(return_statement);
}

static Node* get_assign_expression()
{
    GET_INTRO();

    CHECK_TOK(TOK_LET,
              "'let' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;

    Node* left = NULL;
    NEXT;

    if (TOKEN == TOK_COLON)
    {
        PREV;
        left = get_name_type();
    }
    else
    {
        PREV;
        left = get_name();
    }

    Node* right = NULL;
    if (TOKEN == TOK_ASSIGNMENT)
    {
        NEXT;
        right = get_expression();
    }

    Node* assign_expression = node_ctor(
        (Node_data) {
            .type = NODE_ASSIGN_EXPRESSION,
        },
        left,
        right);

    GET_OUTRO(assign_expression);
}

static Node* get_math_expression()
{
    GET_INTRO();
    GET_OUTRO(get_X());
}

static Node* get_X()
{
    GET_INTRO();

    Node* O = get_O();
    while (TOKEN == TOK_XOR)
    {
        NEXT;
        Node* next_O = get_O();

        O = OPERATION(M_XOR, O, next_O);
    }

    GET_OUTRO(O);
}

static Node* get_O()
{
    GET_INTRO();

    Node* CMP = get_CMP();
    while (TOKEN == TOK_OR)
    {
        NEXT;
        Node* next_CMP = get_CMP();

        CMP = OPERATION(M_OR, CMP, next_CMP);
    }

    GET_OUTRO(CMP);
}

static Node* get_CMP()
{
    GET_INTRO();

    Node* A = get_A();

    Token_type type = TOKEN;
    while (TOK_EQUAL <= type && type <= TOK_LESS_EQUAL)
    {
        log_info("cmp go on, %u", type);
        NEXT;

        Math_operation op = {};
        switch (type)
        {
            case TOK_EQUAL:
                op = M_EQUAL;
                break;
            case TOK_GREATER:
                op = M_GREATER;
                break;
            case TOK_GREATER_EQUAL:
                op = M_GREATER_EQUAL;
                break;
            case TOK_LESS:
                op = M_LESS;
                break;
            case TOK_LESS_EQUAL:
                op = M_LESS_EQUAL;
                break;
            default:
                THROW(ERROR_SYNTAX,
                      "compare operator expected, got %s",
                      token_to_string(*fe_tokens).data);
        }

        Node* next_A = get_A();

        A = OPERATION(op, A, next_A);
        type = TOKEN;
    }

    GET_OUTRO(A);
}

static Node* get_A()
{
    GET_INTRO();

    Node* AS = get_AS();
    while (TOKEN == TOK_AND)
    {
        NEXT;
        Node* next_AS = get_AS();

        AS = OPERATION(M_AND, AS, next_AS);
    }

    GET_OUTRO(AS);
}

static Node* get_AS()
{
    GET_INTRO();

    Node* E = get_E();
    while (TOKEN == TOK_ASSIGNMENT)
    {
        NEXT;
        Node* next_E = get_E();

        E = OPERATION(M_ASSIGN, E, next_E);
    }

    GET_OUTRO(E);
}

static Node* get_E()
{
    GET_INTRO();

    Node* T = get_T();

    Token_type type = TOKEN;
    while (type == TOK_PLUS || type == TOK_MINUS)
    {
        NEXT;
        Node* next_T = get_T();

        T = OPERATION(type == TOK_PLUS ? M_PLUS : M_MINUS, T, next_T);
        type = TOKEN;
    }

    GET_OUTRO(T);
}

static Node* get_T()
{
    GET_INTRO();

    Node* D = get_D();

    Token_type type = TOKEN;
    while (type == TOK_MULTIPLY || type == TOK_DIVIDE)
    {
        NEXT;
        Node* next_D = get_D();

        D = OPERATION(type == TOK_MULTIPLY ? M_MULTIPLY : M_DIVIDE, D, next_D);
        type = TOKEN;
    }

    GET_OUTRO(D);
}

static Node* get_D()
{
    GET_INTRO();

    Node* P = get_P();
    Node* prev = P;

    while (TOKEN == TOK_POWER)
    {
        NEXT;
        Node* next_P = get_P();

        Node* prev_copy = node_copy(prev);
        *prev = (Node) {N(NODE_MATH_OPERATION), prev_copy, next_P};
        prev->data.operation = M_EXPONENT;
        prev->left = prev_copy;
        prev->right = next_P;

        prev = next_P;
    }

    GET_OUTRO(P);
}

static Node* get_P()
{
    GET_INTRO();

    Node* P = NULL;

    Token_type type = TOKEN;
    switch (type)
    {
        case TOK_MINUS:
        case TOK_NOT:
        {
            NEXT;
            Node* P = get_P();

            if (type == TOK_MINUS)
                P = OPERATION(M_MULTIPLY, INTEGER(-1), P);
            else
                P = OPERATION(M_NOT, P, NULL);
            break;
        }
        case TOK_OPEN_BRACKET:
        {
            NEXT;
            Node* expression = get_expression();
            CHECK_TOK(TOK_CLOSE_BRACKET,
                      "')' expected, got %s",
                      token_to_string(*fe_tokens).data);
            NEXT;
            P = expression;
            break;
        }
        default:
            P = get_identifier();
    }

    GET_OUTRO(P);
}

static Node* get_identifier()
{
    GET_INTRO();

    Node* identifier = NULL;

    switch (TOKEN)
    {
        case TOK_INTEGER:
        case TOK_FLOAT:
            identifier = get_number();
            break;
        default:
            identifier = get_name();
    }

    GET_OUTRO(identifier);
}

static Node* get_name_type()
{
    GET_INTRO();

    CHECK_TOK(TOK_NAME,
              "name expected, got %s",
              token_to_string(*fe_tokens).data);
    String name = fe_tokens->string;
    NEXT;
    CHECK_TOK(TOK_COLON,
              "':' expected, got %s",
              token_to_string(*fe_tokens).data);
    NEXT;
    String type = fe_tokens->string;
    NEXT;

    Node* name_type = node_ctor(N(NODE_NAME_TYPE), NAME(name), NAME(type));

    GET_OUTRO(name_type);
}

static Node* get_name()
{
    GET_INTRO();

    CHECK_TOK(TOK_NAME,
              "name expected, got %s",
              token_to_string(*fe_tokens).data);
    Node* name =
        (node_ctor((Node_data) {.type = NODE_NAME, .string = fe_tokens->string},
                   ((void*)0),
                   ((void*)0)));
    NEXT;

    GET_OUTRO(name);
}

static Node* get_string()
{
    GET_INTRO();

    CHECK_TOK(TOK_STRING,
              "string expected, got %s",
              token_to_string(*fe_tokens).data);
    Node* string = STRING(fe_tokens->string);
    NEXT;

    GET_OUTRO(string);
}

static Node* get_number()
{
    GET_INTRO();

    Node* number = NULL;
    switch (TOKEN)
    {
        case TOK_INTEGER:
            number = INTEGER(fe_tokens->integer);
            break;
        case TOK_FLOAT:
            number = FLOATING(fe_tokens->floating);
            break;
        default:
            THROW(ERROR_SYNTAX,
                  "integer expected, got %s",
                  token_to_string(*fe_tokens).data);
    }
    NEXT;

    GET_OUTRO(number);
}
