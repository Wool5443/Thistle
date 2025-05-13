#include "Tokens.h"
#include "Common.h"

Str token_literal(Token_type token)
{
    switch (token)
    {
    case TOK_IF:
        return STR_LITERAL("if");
    case TOK_ELSE:
        return STR_LITERAL("else");
    case TOK_WHILE:
        return STR_LITERAL("while");
    case TOK_FOR:
        return STR_LITERAL("for");
    case TOK_FN:
        return STR_LITERAL("fn");
    case TOK_BREAK:
        return STR_LITERAL("break");
    case TOK_LET:
        return STR_LITERAL("let");
    case TOK_RETURN:
        return STR_LITERAL("return");
    case TOK_OPEN_BRACKET:
        return STR_LITERAL("(");
    case TOK_CLOSE_BRACKET:
        return STR_LITERAL(")");
    case TOK_OPEN_SCOPE:
        return STR_LITERAL("{");
    case TOK_CLOSE_SCOPE:
        return STR_LITERAL("}");
    case TOK_MARK:
        return STR_LITERAL("\"");
    case TOK_COMMA:
        return STR_LITERAL(",");
    case TOK_DOT:
        return STR_LITERAL(".");
    case TOK_SEMI_COLON:
        return STR_LITERAL(";");
    case TOK_COLON:
        return STR_LITERAL(":");
    case TOK_PLUS:
        return STR_LITERAL("+");
    case TOK_MINUS:
        return STR_LITERAL("-");
    case TOK_MULTIPLY:
        return STR_LITERAL("*");
    case TOK_DIVIDE:
        return STR_LITERAL("/");
    case TOK_POWER:
        return STR_LITERAL("^");
    case TOK_TRUE:
        return STR_LITERAL("true");
    case TOK_FALSE:
        return STR_LITERAL("false");
    case TOK_NOT:
        return STR_LITERAL("!");
    case TOK_AND:
        return STR_LITERAL("and");
    case TOK_OR:
        return STR_LITERAL("or");
    case TOK_XOR:
        return STR_LITERAL("xor");
    case TOK_EQUAL:
        return STR_LITERAL("==");
    case TOK_NOT_EQUAL:
        return STR_LITERAL("!=");
    case TOK_GREATER:
        return STR_LITERAL(">");
    case TOK_GREATER_EQUAL:
        return STR_LITERAL(">=");
    case TOK_LESS:
        return STR_LITERAL("<");
    case TOK_LESS_EQUAL:
        return STR_LITERAL("<=");
    case TOK_ASSIGNMENT:
        return STR_LITERAL("=");
    case TOK_NAME:
        return STR_LITERAL("TOK_NAME");
    case TOK_STRING:
        return STR_LITERAL("TOK_STRING");
    case TOK_INTEGER:
        return STR_LITERAL("TOK_INTEGER");
    case TOK_END:
        return STR_LITERAL("TOK_END");
    case TOK_BAD:
        return STR_LITERAL("TOK_BAD");
    case KEYWORD_COUNT:
        return STR_LITERAL("KEYWORD_COUNT");
    default:
        return STR_LITERAL("UNKNOWN TOKEN TYPE");
    }
}

String token_to_string(Token token)
{
    switch (token.type)
    {
    case TOK_INTEGER:
        return TRY_RES(string_ctor_printf("%d", token.integer));
    case TOK_NAME:
        return TRY_RES(string_ctor_printf("NAME<%s>", token.string.data));
    case TOK_STRING:
        return TRY_RES(string_ctor_printf("STRING<%s>", token.string.data));
    default:
        return TRY_RES(string_ctor_printf("%s", token_literal(token.type).data));
    }
}
