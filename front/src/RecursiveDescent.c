#include "RecursiveDescent.h"
#include "DSL.h"

#define S assert(tokens)

#define CHECK_TOK(t, ...) if (TOKEN != t) THROW(ERROR_SYNTAX __VA_OPT__(,) __VA_ARGS__)
#define TOKEN (fe_tokens->type)
#define NEXT (++fe_tokens)
#define PREV (--fe_tokens)

#define GET_INTRO() log_debug("%s, current token: %s", __PRETTY_FUNCTION__, token_to_string(*fe_tokens).data)
// #define GET_INTRO()

static Tokens fe_tokens;

static Node* get_g();
static Node* get_function();
static Node* get_function_body();
static Node* get_function_signature();
static Node* get_function_signature_args();
static Node* get_statement();
static Node* get_expression();
static Node* get_function_call();
static Node* get_function_call_args();
static Node* get_return_statement();
static Node* get_print_expression();
static Node* get_assign_expression();
static Node* get_math_expression();
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
    Node* G = node_ctor(
        N(NODE_G), function, NULL
    );

    while (TOKEN != TOK_END)
    {
        Node* next = get_function();
        G = node_ctor(
            N(NODE_G), G, next
        );
    }

    return G;
}

static Node* get_function()
{
    GET_INTRO();

    CHECK_TOK(TOK_FN, "'fn' expected, got %s", token_to_string(*fe_tokens).data);

    Node* function_signature = get_function_signature();

    switch (TOKEN)
    {
        case TOK_SEMI_COLON:
        {
            NEXT;
            Node* function = node_ctor(
                N(NODE_FUNCTION), function_signature, NULL
            );
            return function;
        }
        case TOK_OPEN_SCOPE:
        {
            Node* function_body = get_function_body();
            Node* function = node_ctor(
                N(NODE_FUNCTION), function_signature, function_body
            );
            return function;
        }
        default:
            THROW(ERROR_SYNTAX, "';' or function body expected, got %s", token_to_string(*fe_tokens).data);
    }
}

static Node* get_function_body()
{
    GET_INTRO();

    CHECK_TOK(TOK_OPEN_SCOPE, "'{' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    Node* statement = get_statement();

    while (TOKEN != TOK_CLOSE_SCOPE)
    {
        Node* next_statement = get_statement();
        statement = node_ctor(
            N(NODE_STATEMENT), statement, next_statement
        );
    }

    CHECK_TOK(TOK_CLOSE_SCOPE, "'}' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    return statement;
}

static Node* get_function_signature()
{
    GET_INTRO();

    CHECK_TOK(TOK_FN, "'fn' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    Node* name = get_name();
    Node* function_signature = node_ctor(
        N(NODE_FUNCTION_SIGNATURE), name, NULL
    );

    CHECK_TOK(TOK_OPEN_BRACKET, "'(' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    if (TOKEN == TOK_CLOSE_BRACKET)
    {
        NEXT;
        return function_signature;
    }

    Node* function_signature_args = get_function_signature_args();
    function_signature->right = function_signature_args;

    return function_signature ;
}

static Node* get_function_signature_args()
{
    GET_INTRO();

    Node* result = get_name_type();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        Node* name_type = get_name_type();
        result = node_ctor(N(NODE_COMMA), result, name_type);
    }

    return result;
}

static Node* get_statement()
{
    GET_INTRO();

    switch (TOKEN)
    {
        case TOK_RETURN:
            return get_return_statement();
        default:
        {
            Node* expression = get_expression();
            CHECK_TOK(TOK_SEMI_COLON, "';' expected, got %s", token_to_string(*fe_tokens).data);
            NEXT;
            return expression;
        }
    }
}

static Node* get_expression()
{
    GET_INTRO();

    switch (TOKEN)
    {
        case TOK_LET:
            return get_assign_expression();
        case TOK_PRINT:
            return get_print_expression();
        case TOK_NAME:
            NEXT;
            if (TOKEN == TOK_OPEN_BRACKET)
            {
                PREV;
                return get_function_call();
            }
            else
            {
                PREV;
                return get_math_expression();
            }
        case TOK_MARK:
            return get_string();
        default:
            return get_math_expression();
    }
}

static Node* get_function_call()
{
    GET_INTRO();

    Node* name = get_name();
    CHECK_TOK(TOK_OPEN_BRACKET, "'(' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    Node* args = get_function_call_args();

    CHECK_TOK(TOK_CLOSE_BRACKET, "')' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    Node* result = node_ctor(N(NODE_FUNCTION_CALL), name, args);

    return result;
}

static Node* get_function_call_args()
{
    GET_INTRO();

    Node* result = get_expression();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        CHECK_TOK(TOK_COMMA, "',' expected, got %s", token_to_string(*fe_tokens).data);
        NEXT;
        Node* expression = get_expression();
        result = node_ctor(N(NODE_COMMA), result, expression);
    }

    return result;
}

static Node* get_return_statement()
{
    GET_INTRO();

    CHECK_TOK(TOK_RETURN, "'return' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    if (TOKEN == TOK_SEMI_COLON)
    {
        NEXT;
        return node_ctor(N(NODE_RETURN_STATEMENT), NULL, NULL);
    }

    Node* expression = get_expression();
    CHECK_TOK(TOK_SEMI_COLON, "';' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;

    return node_ctor(N(NODE_RETURN_STATEMENT), expression, NULL);
}

static Node* get_print_expression()
{
    GET_INTRO();

    CHECK_TOK(TOK_PRINT);
    NEXT;
    CHECK_TOK(TOK_OPEN_BRACKET);
    NEXT;

    Node* function_call_args = get_function_call_args();

    Node* print_expression = node_ctor(
        N(NODE_PRINT_EXPRESSION),
        function_call_args,
        NULL
    );

    CHECK_TOK(TOK_CLOSE_BRACKET);
    NEXT;

    return print_expression;
}

static Node* get_assign_expression()
{
    GET_INTRO();

    CHECK_TOK(TOK_LET, "'let' expected, got %s", token_to_string(*fe_tokens).data);
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
        right
    );

    return assign_expression;
}

static Node* get_math_expression()
{
    GET_INTRO();

    Node* T = get_T();

    Token_type type = TOKEN;
    while (type == TOK_PLUS || type == TOK_MINUS)
    {
        NEXT;
        Node* next_T = get_T();

        T = OPERATION(type == TOK_PLUS ? M_PLUS : M_MINUS,
                      T, next_T);
        type = TOKEN;
    }

    return T;
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

        D = OPERATION(type == TOK_MULTIPLY ? M_MULTIPLY : M_DIVIDE,
                      D, next_D);
        type = TOKEN;
    }

    return D;
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
        *prev = (Node){ N(NODE_MATH_OPERATION), prev_copy, next_P };
        prev->data.operation = M_EXPONENT;
        prev->left = prev_copy;
        prev->right = next_P;

        prev = next_P;
    }

    return P;
}

static Node* get_P()
{
    GET_INTRO();

    switch (TOKEN)
    {
        case TOK_MINUS:
        {
            NEXT;
            Node* P = get_P();
            Node* result = OPERATION(M_MULTIPLY, NUMBER(-1), P);
            return result;
        }
        case TOK_OPEN_BRACKET:
        {
            NEXT;
            Node* expression = get_expression();
            CHECK_TOK(TOK_CLOSE_BRACKET, "')' expected, got %s", token_to_string(*fe_tokens).data);
            NEXT;
            return expression;
        }
        default:
            return get_identifier();
    }
}

static Node* get_identifier()
{
    GET_INTRO();

    if (TOKEN == TOK_INTEGER)
        return get_number();
    return get_name();
}

static Node* get_name_type()
{
    GET_INTRO();

    CHECK_TOK(TOK_NAME, "name expected, got %s", token_to_string(*fe_tokens).data);
    String name = fe_tokens->string;
    NEXT;
    CHECK_TOK(TOK_COLON, "':' expected, got %s", token_to_string(*fe_tokens).data);
    NEXT;
    String type = fe_tokens->string;
    NEXT;

    Node* name_type = node_ctor(
        N(NODE_NAME_TYPE),
        NAME(name),
        STRING(type)
    );

    return name_type;
}

static Node* get_name()
{
    GET_INTRO();

    CHECK_TOK(TOK_NAME, "name expected, got %s", token_to_string(*fe_tokens).data);
    Node* name = (node_ctor((Node_data){.type = NODE_NAME, .string = fe_tokens->string}, ((void*)0), ((void*)0)));
    NEXT;
    return name;
}

static Node* get_string()
{
    GET_INTRO();

    CHECK_TOK(TOK_STRING, "string expected, got %s", token_to_string(*fe_tokens).data);
    Node* string = STRING(fe_tokens->string);
    NEXT;
    return string;
}

static Node* get_number()
{
    GET_INTRO();

    CHECK_TOK(TOK_INTEGER, "integer expected, got %s", token_to_string(*fe_tokens).data);
    Node* number = NUMBER(fe_tokens->integer);
    NEXT;
    return number;
}
