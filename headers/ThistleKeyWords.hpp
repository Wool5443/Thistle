#pragma once

// DEF_KEYWORD(text, name)

DEF_KEYWORD("стп",       END_STATEMENT)

DEF_KEYWORD("чсл",       DOUBLE)
DEF_KEYWORD("ткст",      STRING)
DEF_KEYWORD("нх",        VOID)

// DEF_KEYWORD("фнкц",   FUNCTION)
// DEF_KEYWORD("врнть",  RETURN)

DEF_KEYWORD("[",         OPEN_BRACKET)
DEF_KEYWORD("]",         CLOSE_BRACKET)

DEF_KEYWORD("нчть",      BLOCK_START)
DEF_KEYWORD("зкнчть",    BLOCK_END)

DEF_KEYWORD("плс",       PLUS)
DEF_KEYWORD("мнс",       MINUS)
DEF_KEYWORD("мнжть",     MULTIPLY)
DEF_KEYWORD("рздлть",    DIVIDE)

DEF_KEYWORD("плсрвн",    PLUS_EQUAL)
DEF_KEYWORD("мнсрвн",    MINUS_EQUAL)
DEF_KEYWORD("мнжтьрвн",  MULTIPLY_EQUAL)
DEF_KEYWORD("рздлтьрвн", DIVIDE_EQUAL)

DEF_KEYWORD("рвн",       EQUALS)
DEF_KEYWORD("нрвн",      NOT_EQUALS)
DEF_KEYWORD("бльш",      MORE)
DEF_KEYWORD("бльшрвн",   MORE_EQUALS)
DEF_KEYWORD("мньш",      LESS)
DEF_KEYWORD("меньшрвн",  LESS_EQUALS)


DEF_KEYWORD("прмн",      VARIABLE)
DEF_KEYWORD("кнстнт",    CONSTANT)

DEF_KEYWORD("нзнчть",    ASSIGN)

DEF_KEYWORD("пк",        WHILE)
