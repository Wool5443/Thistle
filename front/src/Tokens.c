#include "Tokens.h"

void token_print(Token token, FILE* out)
{
    switch (token.type)
    {
        case TOK_IMMEDIATE:
            fprintf(out, "%d", token.immed);
            break;
        case TOK_NAME:
            fprintf(out, "%s", token.name.data);
            break;
        case TOK_END:
            fprintf(out, "TOK_END");
            break;
        case TOK_BAD:
            fprintf(out, "TOK_BAD");
            break;
        case TOK_OPEN_SCOPE:
            fprintf(out, "TOK_OPEN_SCOPE");
            break;
        case TOK_CLOSE_SCOPE:
            fprintf(out, "TOK_CLOSE_SCOPE");
            break;
        case TOK_BLOCK:
            fprintf(out, "TOK_BLOCK");
            break;
        case KEYWORD_COUNT:
            fprintf(out, "KEYWORD COUNT");
            break;
        default:
            fprintf(out, "%s", token_entries[token.type].text.data);
            break;
    }
}
