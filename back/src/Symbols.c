#include "Symbols.h"

#include "Common.h"

void symbol_add(Symbol_table* table, String name)
{
    if (!table)
    {
        THROW(ERROR_NULLPTR, "NULL passed as table");
    }

    TRY(vec_add(*table, (Symbol) {name}));
}
