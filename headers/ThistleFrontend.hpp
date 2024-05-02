#pragma once

#include "Tree.hpp"

enum Keywords
{
    #define DEF_KEYWORD(text, name, ...) name,

    #include "ThistleKeyWords.hpp"

    #undef DEF_KEYWORD
};

constexpr char* TOKENS[] =
{
    #define DEF_KEYWORD(text, ...) name,

    #include "ThistleKeyWords.hpp"

    #undef DEF_KEYWORD
};

enum TokenType
{
    ASSIGN,
    BLOCK,
    MATH_OPERATION,
};

struct ThistleToken
{
    const char*      funcName;
    const char*      text;
    size_t           lineNumber;
};

ErrorCode Parse(Tree* tree, String* string);
