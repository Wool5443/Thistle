#ifndef THISTLE_TOKENS_H_
#define THISTLE_TOKENS_H_

#include "Vector.h" // IWYU pragma: export
#include "String.h"

typedef enum Token_type
{
    TOK_BAD,
    TOK_IF,
    TOK_WHILE,
    TOK_FOR,
    TOK_BREAK,
    TOK_LET,
    TOK_OPEN_BRACKET,
    TOK_CLOSE_BRACKET,
    TOK_OPEN_SCOPE,
    TOK_CLOSE_SCOPE,
    TOK_COMMA,
    TOK_DOT,
    TOK_SEMI_COLON,
    TOK_INT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_POWER,
    TOK_EQUALS,
    TOK_GREATER,
    TOK_GREATER_EQUAL,
    TOK_LESS,
    TOK_LESS_EQUAL,
    TOK_ASSIGNMENT,

    KEYWORD_COUNT,

    TOK_NAME,
    TOK_IMMEDIATE,
} Token_type;

typedef struct Token_entry
{
    Token_type type;
    Str text;
} Token_entry;

static Token_entry token_entries[] = {
    {},
    { TOK_IF,            STR_LITERAL("if")    },
    { TOK_WHILE,         STR_LITERAL("while") },
    { TOK_FOR,           STR_LITERAL("for")   },
    { TOK_BREAK,         STR_LITERAL("break") },
    { TOK_LET,           STR_LITERAL("let")   },
    { TOK_OPEN_BRACKET,  STR_LITERAL("(")     },
    { TOK_CLOSE_BRACKET, STR_LITERAL(")")     },
    { TOK_OPEN_SCOPE,    STR_LITERAL("{")     },
    { TOK_CLOSE_SCOPE,   STR_LITERAL("}")     },
    { TOK_COMMA,         STR_LITERAL(",")     },
    { TOK_DOT,           STR_LITERAL(".")     },
    { TOK_SEMI_COLON,    STR_LITERAL(",")     },
    { TOK_INT,           STR_LITERAL("int")   },
    { TOK_PLUS,          STR_LITERAL("+")     },
    { TOK_MINUS,         STR_LITERAL("-")     },
    { TOK_MULTIPLY,      STR_LITERAL("*")     },
    { TOK_DIVIDE,        STR_LITERAL("/")     },
    { TOK_POWER,         STR_LITERAL("^")     },
    { TOK_EQUALS,        STR_LITERAL("==")    },
    { TOK_GREATER,       STR_LITERAL(">")     },
    { TOK_GREATER,       STR_LITERAL(">=")    },
    { TOK_LESS,          STR_LITERAL("<")     },
    { TOK_LESS,          STR_LITERAL("<=")    },
    { TOK_ASSIGNMENT,    STR_LITERAL("=")     },
    {},
    { TOK_NAME, {} },
    { TOK_IMMEDIATE, {} },
};

typedef struct Token
{
    union
    {
        int immed;
        String name;
    };
    Token_type type;
} Token;

typedef Token* Tokens;

void token_print(Token token, FILE* out);

#endif // THISTLE_TOKENS_H_
