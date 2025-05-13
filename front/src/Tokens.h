#ifndef THISTLE_TOKENS_H_
#define THISTLE_TOKENS_H_

#include <stdint.h>

#include "Vector.h" // IWYU pragma: export
#include "String.h"
#include "FrontCommon.h"

typedef enum Token_type
{
    TOK_BAD,

    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_FOR,

    TOK_FN,
    TOK_BREAK,
    TOK_LET,
    TOK_RETURN,

    TOK_OPEN_BRACKET,
    TOK_CLOSE_BRACKET,

    TOK_OPEN_SCOPE,
    TOK_CLOSE_SCOPE,

    TOK_MARK,

    TOK_COMMA,
    TOK_DOT,

    TOK_SEMI_COLON,
    TOK_COLON,

    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_POWER,

    TOK_TRUE,
    TOK_FALSE,
    TOK_NOT,
    TOK_AND,
    TOK_OR,
    TOK_XOR,

    TOK_EQUAL,
    TOK_NOT_EQUAL,
    TOK_GREATER,
    TOK_GREATER_EQUAL,
    TOK_LESS,
    TOK_LESS_EQUAL,

    TOK_ASSIGNMENT,

    KEYWORD_COUNT,

    TOK_NAME,
    TOK_STRING,
    TOK_INTEGER,
    TOK_END,
} Token_type;

typedef struct Token_entry
{
    Token_type type;
    Str text;
} Token_entry;

typedef struct Token
{
    union
    {
        int     integer;
        String  string;
    };
    Token_type type;
} Token;

typedef Token* Tokens;

INLINE Str token_literal(Token_type token);
INLINE String token_to_string(Token token);

INLINE Str token_literal(Token_type token)
{
    switch (token)
    {
        case TOK_IF:
            return STR_LITERAL("if");
        case TOK_ELSE:
            return STR_LITERAL("else");
        case TOK_WHILE:
            return STR_LITERAL("while");
        case TOK_FOR:
            return STR_LITERAL("for");
        case TOK_FN:
            return STR_LITERAL("fn");
        case TOK_BREAK:
            return STR_LITERAL("break");
        case TOK_LET:
            return STR_LITERAL("let");
        case TOK_RETURN:
            return STR_LITERAL("return");
        case TOK_OPEN_BRACKET:
            return STR_LITERAL("(");
        case TOK_CLOSE_BRACKET:
            return STR_LITERAL(")");
        case TOK_OPEN_SCOPE:
            return STR_LITERAL("{");
        case TOK_CLOSE_SCOPE:
            return STR_LITERAL("}");
        case TOK_MARK:
            return STR_LITERAL("\"");
        case TOK_COMMA:
            return STR_LITERAL(",");
        case TOK_DOT:
            return STR_LITERAL(".");
        case TOK_SEMI_COLON:
            return STR_LITERAL(";");
        case TOK_COLON:
            return STR_LITERAL(":");
        case TOK_PLUS:
            return STR_LITERAL("+");
        case TOK_MINUS:
            return STR_LITERAL("-");
        case TOK_MULTIPLY:
            return STR_LITERAL("*");
        case TOK_DIVIDE:
            return STR_LITERAL("/");
        case TOK_POWER:
            return STR_LITERAL("^");
        case TOK_TRUE:
            return STR_LITERAL("true");
        case TOK_FALSE:
            return STR_LITERAL("false");
        case TOK_NOT:
            return STR_LITERAL("!");
        case TOK_AND:
            return STR_LITERAL("and");
        case TOK_OR:
            return STR_LITERAL("or");
        case TOK_XOR:
            return STR_LITERAL("xor");
        case TOK_EQUAL:
            return STR_LITERAL("==");
        case TOK_NOT_EQUAL:
            return STR_LITERAL("!=");
        case TOK_GREATER:
            return STR_LITERAL(">");
        case TOK_GREATER_EQUAL:
            return STR_LITERAL(">=");
        case TOK_LESS:
            return STR_LITERAL("<");
        case TOK_LESS_EQUAL:
            return STR_LITERAL("<=");
        case TOK_ASSIGNMENT:
            return STR_LITERAL("=");
        case TOK_NAME:
            return STR_LITERAL("TOK_NAME");
        case TOK_STRING:
            return STR_LITERAL("TOK_STRING");
        case TOK_INTEGER:
            return STR_LITERAL("TOK_INTEGER");
        case TOK_END:
            return STR_LITERAL("TOK_END");
        case TOK_BAD:
            return STR_LITERAL("TOK_BAD");
        case KEYWORD_COUNT:
            return STR_LITERAL("KEYWORD_COUNT");
        default:
            return STR_LITERAL("UNKNOWN TOKEN TYPE");
    }
}

INLINE String token_to_string(Token token)
{
    switch (token.type)
    {
        case TOK_INTEGER:
            return TRY_RES(string_printf("%d", token.integer));
        case TOK_NAME:
            return TRY_RES(string_printf("NAME<%s>", token.string.data));
        case TOK_STRING:
            return TRY_RES(string_printf("STRING<%s>", token.string.data));
        default:
            return TRY_RES(string_printf("%s", token_literal(token.type).data));
    }
}

#endif // THISTLE_TOKENS_H_
