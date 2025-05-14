#include "Common.h"
#include "Symbols.h"

void symbol_add(Symbol_table* table, String name)
{
    if (!table)
    {
        THROW(ERROR_NULLPTR, "NULL passed as table");
    }

    TRY(vec_add(*table, (Symbol){ name }));
}
