#include "RecursiveDescent.h"
#include "DSL.h"

#define S assert(tokens)

#define CHECK_TOK(t, ...) if (TOKEN != t) THROW(ERROR_SYNTAX __VA_OPT__(,) __VA_ARGS__)
#define TOKEN (fe_tokens->type)
#define NEXT (++fe_tokens)
#define PREV (--fe_tokens)

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
    auto function = get_function();
    auto G = node_ctor(
        N(NODE_G), function, NULL
    );

    while (TOKEN != TOK_END)
    {
        auto next = get_function();
        G = node_ctor(
            N(NODE_G), G, next
        );
    }

    return G;
}

static Node* get_function()
{
    CHECK_TOK(TOK_FN, "'fn' expected");

    auto function_signature = get_function_signature();

    switch (TOKEN)
    {
        case TOK_SEMI_COLON:
        {
            NEXT;
            auto function = node_ctor(
                N(NODE_FUNCTION), function_signature, NULL
            );
            return function;
        }
        case TOK_OPEN_SCOPE:
        {
            NEXT;
            auto function_body = get_function_body();
            auto function = node_ctor(
                N(NODE_FUNCTION), function_signature, function_body
            );
            return function;
        }
        default:
            THROW(ERROR_SYNTAX, "';' or function body expected");
    }
}

static Node* get_function_body()
{
    CHECK_TOK(TOK_OPEN_SCOPE, "'{' expected");

    auto statement = get_statement();

    while (TOKEN != TOK_CLOSE_SCOPE)
    {
        auto next_statement = get_statement();
        statement = node_ctor(
            N(NODE_STATEMENT), statement, next_statement
        );
    }

    CHECK_TOK(TOK_CLOSE_SCOPE, "'}' expected");

    return statement;
}

static Node* get_function_signature()
{
    CHECK_TOK(TOK_FN, "'fn' expected");

    auto name = get_name();
    auto function_signature = node_ctor(
        N(NODE_FUNCTION_SIGNATURE), name, NULL
    );

    CHECK_TOK(TOK_OPEN_BRACKET, "'(' expected"); NEXT;
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
    auto result = get_name_type();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        auto name_type = get_name_type();
        result = node_ctor(N(NODE_COMMA), result, name_type);
    }

    return result;
}

static Node* get_statement()
{
    switch (TOKEN)
    {
        case TOK_RETURN:
            return get_return_statement();
        default:
        {
            auto expression = get_expression();
            CHECK_TOK(TOK_SEMI_COLON, "';' expected");
            return expression;
        }
    }
}

static Node* get_expression()
{
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
            THROW(ERROR_SYNTAX);
    }
}

static Node* get_function_call()
{
    auto name = get_name();
    CHECK_TOK(TOK_OPEN_BRACKET); NEXT;

    auto args = get_function_call_args();

    CHECK_TOK(TOK_CLOSE_BRACKET); NEXT;

    auto result = node_ctor(N(NODE_FUNCTION_CALL), name, args);

    return result;
}

static Node* get_function_call_args()
{
    auto result = get_expression();

    while (TOKEN != TOK_CLOSE_BRACKET)
    {
        CHECK_TOK(TOK_COMMA); NEXT;
        auto expression = get_expression();
        result = node_ctor(N(NODE_COMMA), result, expression);
    }

    return result;
}

static Node* get_return_statement()
{
    CHECK_TOK(TOK_RETURN); NEXT;

    if (TOKEN == TOK_SEMI_COLON)
    {
        NEXT;
        return node_ctor(N(NODE_RETURN_STATEMENT), NULL, NULL);
    }

    auto expression = get_expression();
    CHECK_TOK(TOK_SEMI_COLON); NEXT;

    return node_ctor(N(NODE_RETURN_STATEMENT), expression, NULL);
}

static Node* get_print_expression()
{
    CHECK_TOK(TOK_PRINT); NEXT;
    CHECK_TOK(TOK_OPEN_BRACKET); NEXT;

    auto function_call_args = get_function_call_args();

    auto print_expression = node_ctor(
        N(NODE_PRINT_EXPRESSION),
        function_call_args,
        NULL
    );

    CHECK_TOK(TOK_CLOSE_BRACKET); NEXT;

    return print_expression;
}

static Node* get_assign_expression()
{
    CHECK_TOK(TOK_LET); NEXT;

    auto left = get_name_type();

    if (!left)
    {
        left = get_name();
    }
    if (!left)
    {
        THROW(ERROR_SYNTAX, "name expected");
    }

    auto right = NULL;
    if (TOKEN == TOK_ASSIGNMENT)
    {
        right = get_expression();
    }

    auto assign_expression = node_ctor(
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
    auto T = get_T();

    auto result = T;

    Token_type type = TOKEN;
    while (type == TOK_PLUS || type == TOK_MINUS)
    {
        NEXT;
        auto next_T = get_T();

        result = OPERATION(type == TOK_PLUS ? M_MULTIPLY : M_MINUS,
                           T, next_T);
    }

    return result;
}

static Node* get_T()
{
    auto D = get_D();

    auto result = D;

    Token_type type = TOKEN;
    while (type == TOK_MULTIPLY || type == TOK_DIVIDE)
    {
        NEXT;
        Node* next_D = get_D();

        result = OPERATION(type == TOK_MULTIPLY ? M_MULTIPLY : M_DIVIDE,
                           D, next_D);
    }

    return result;
}

static Node* get_D()
{
    auto P = get_P();

    auto result = P;

    while (TOKEN == TOK_POWER)
    {
        NEXT;
        auto next_P = get_P();

        result = OPERATION(M_EXPONENT, P, next_P);
    }

    return result;
}

static Node* get_P()
{
    switch (TOKEN)
    {
        case TOK_MINUS:
        {
            NEXT;
            auto P = get_P();
            auto result = OPERATION(M_MULTIPLY, NUMBER(-1), P);
            return result;
        }
        case TOK_OPEN_BRACKET:
        {
            NEXT;
            auto expression = get_expression();
            CHECK_TOK(TOK_CLOSE_BRACKET); NEXT;
            return expression;
        }
        default:
            return get_identifier();
    }
}

static Node* get_identifier()
{
    auto number = get_number();
    if (number)
    {
        return number;
    }

    return get_name();
}

static Node* get_name_type()
{
    CHECK_TOK(TOK_NAME);
    String name = fe_tokens->string;
    NEXT;
    CHECK_TOK(TOK_COLON); NEXT;
    String type = fe_tokens->string;
    NEXT;

    auto name_type = node_ctor(
        N(NODE_NAME_TYPE),
        STRING(name),
        STRING(type)
    );

    return name_type;
}

static Node* get_name()
{
    CHECK_TOK(TOK_NAME);

    auto name = node_ctor(
        (Node_data) {
            .type = NODE_NAME, .string = fe_tokens->string
        },
        NULL,
        NULL
    );

    NEXT;
    return name;
}

static Node* get_string()
{
    CHECK_TOK(TOK_STRING);
    auto string = STRING(fe_tokens->string);
    NEXT;
    return string;
}

static Node* get_number()
{
    CHECK_TOK(TOK_INTEGER);
    auto number = node_ctor(
        (Node_data) {
            .type = NODE_NUMBER, .integer = fe_tokens->integer
        },
        NULL,
        NULL
    );
    NEXT;
    return number;
}
