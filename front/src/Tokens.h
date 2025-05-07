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

[[maybe_unused]] static Token_entry token_entries[] = {
    {},
    { TOK_IF,            STR_LITERAL("if")        },
    { TOK_ELSE,          STR_LITERAL("else")      },
    { TOK_WHILE,         STR_LITERAL("while")     },
    { TOK_FOR,           STR_LITERAL("for")       },

    { TOK_FN,            STR_LITERAL("fn")        },
    { TOK_BREAK,         STR_LITERAL("break")     },
    { TOK_LET,           STR_LITERAL("let")       },
    { TOK_RETURN,        STR_LITERAL("return")    },

    { TOK_OPEN_BRACKET,  STR_LITERAL("(")         },
    { TOK_CLOSE_BRACKET, STR_LITERAL(")")         },

    { TOK_OPEN_SCOPE,    STR_LITERAL("{")         },
    { TOK_CLOSE_SCOPE,   STR_LITERAL("}")         },

    { TOK_MARK,          STR_LITERAL("\"")        },

    { TOK_COMMA,         STR_LITERAL(",")         },
    { TOK_DOT,           STR_LITERAL(".")         },

    { TOK_SEMI_COLON,    STR_LITERAL(";")         },
    { TOK_COLON,         STR_LITERAL(":")         },

    { TOK_PLUS,          STR_LITERAL("+")         },
    { TOK_MINUS,         STR_LITERAL("-")         },
    { TOK_MULTIPLY,      STR_LITERAL("*")         },
    { TOK_DIVIDE,        STR_LITERAL("/")         },
    { TOK_POWER,         STR_LITERAL("^")         },

    { TOK_TRUE,          STR_LITERAL("true")      },
    { TOK_FALSE,         STR_LITERAL("false")     },
    { TOK_NOT,           STR_LITERAL("!")         },
    { TOK_AND,           STR_LITERAL("and")       },
    { TOK_OR,            STR_LITERAL("or")        },
    { TOK_XOR,           STR_LITERAL("xor")       },
    { TOK_EQUAL,         STR_LITERAL("==")        },
    { TOK_GREATER,       STR_LITERAL(">")         },
    { TOK_GREATER_EQUAL, STR_LITERAL(">=")        },
    { TOK_LESS,          STR_LITERAL("<")         },
    { TOK_LESS_EQUAL,    STR_LITERAL("<=")        },

    { TOK_ASSIGNMENT,    STR_LITERAL("=")         },
    {},
    { TOK_NAME, {} },
    { TOK_STRING, {} },
    { TOK_INTEGER,  {} },
    { TOK_END, STR_LITERAL("TOK_END") },
    { TOK_BAD, STR_LITERAL("TOK_BAD") },
    { KEYWORD_COUNT, STR_LITERAL("KEYWORD_COUNT") },
};

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

String token_to_string(Token token);

#endif // THISTLE_TOKENS_H_
