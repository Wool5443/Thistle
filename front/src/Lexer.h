#ifndef THISTLE_LEXER_H_
#define THISTLE_LEXER_H_

#include "Vector.h" // IWYU pragma: export
#include "Tokens.h"

typedef Token* Tokens;

DECLARE_RESULT_HEADER(Tokens);

Tokens tokenize(Str text);

#endif // THISTLE_LEXER_H_
