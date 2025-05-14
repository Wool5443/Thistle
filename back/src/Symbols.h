#ifndef THISTLE_SYMBOLS_H_
#define THISTLE_SYMBOLS_H_

#include "String.h"
#include "Vector.h" // IWYU pragma: export

typedef struct Symbol Symbol;
struct Symbol
{
    String name;
};

typedef Symbol* Symbol_table;

void symbol_add(Symbol_table* table, String name);

#endif // THISTLE_SYMBOLS_H_
