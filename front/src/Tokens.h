#ifndef THISTLE_TOKENS_H_
#define THISTLE_TOKENS_H_

#include <stdint.h>

#include "Vector.h" // IWYU pragma: export
#include "String.h"

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

Str token_literal(Token_type token);
String token_to_string(Token token);

#endif // THISTLE_TOKENS_H_
