#include "Tokens.h"
#include "FrontCommon.h"

String token_to_string(Token token)
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
            return TRY_RES(string_printf("%s", token_entries[token.type].text.data));
    }
}
