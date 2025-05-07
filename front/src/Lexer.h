#ifndef THISTLE_LEXER_H_
#define THISTLE_LEXER_H_

#include "Tokens.h"

Tokens tokenize(Str text);
void delete_comments(String text);

#endif // THISTLE_LEXER_H_
