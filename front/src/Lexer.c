#include <ctype.h>

#include "Lexer.h"
#include "FrontCommon.h"

#define TRY(expr) {auto e = (expr); if (e) THROW(e);}

#define TRY_RES(result)                     \
({                                          \
    auto t = (result);                      \
    if (t.error_code)                       \
        THROW(t.error_code);                \
    t.value;                                \
})

static Tokens tokenize_word(Str word);
static Str* split(Str text);
static Token read_name(const char** text);
static Token read_immed(const char** text);
static Token read_keyword(const char** text);

Tokens tokenize(Str text)
{
    Str* words = split(text);
    Tokens tokens = {};

    for (size_t i = 0, end = vec_size(words); i < end; i++)
    {
        Tokens ts = tokenize_word(words[i]);
        for (size_t j = 0, endj = vec_size(ts); j < endj; j++)
        {
            TRY(vec_add(tokens, ts[j]));
        }
    }

    return tokens;
}

static Tokens tokenize_word(Str word)
{
    Tokens tokens = {};

    const char* p = word.data;
    const char* end = p + word.size;

    while (p < end)
    {

#define ATTEMPT_READ(f)                     \
({                                          \
    Token val = f(&p);                      \
    if (val.type != TOK_BAD)                \
    {                                       \
        TRY(vec_add(tokens, val));          \
        continue;                           \
    }                                       \
})

    ATTEMPT_READ(read_immed);
    ATTEMPT_READ(read_keyword);
    ATTEMPT_READ(read_name);

#undef ATTEMPT_READ

    }

    return tokens;
}

static Str* split(Str text)
{
    static const char* WHITE_SPACE = " \n\r\t\v\f";

    Str* words = NULL;
    const char* prev = text.data;
    const char* curr = NULL;

    while ((curr = strpbrk(prev, WHITE_SPACE)))
    {
        TRY(vec_add(words, str_ctor_size(prev, curr - prev)));
        prev = curr + 1;
    }

    return words;
}

static Token read_name(const char** text)
{
    if (!text) THROW(ERROR_NULLPTR);

    const char* ptr = *text;

    String name = TRY_RES(string_ctor_capacity(1));

    while (!isspace(*ptr))
    {
        TRY(string_append_char(&name, *ptr++));
    }

    *text = ptr;

    return (Token) {
        .type = TOK_NAME,
        .name = name,
    };
}

static Token read_immed(const char** text)
{
    if (!text) THROW(ERROR_NULLPTR);

    char* endp = NULL;
    int immed = strtol(*text, &endp, 0);

    if (immed == 0 &&
       ((endp == *text) ||
        (*endp != '\0' && !isspace(*endp))))
    {
        return (Token){};
    }

    *text = endp;

    return (Token) {
        .type = TOK_IMMEDIATE,
        .immed = immed,
    };
}

static Token read_keyword(const char** text)
{
    if (!text) THROW(ERROR_NULLPTR);

    size_t len = strlen(*text);

    for (int i = 1; i < KEYWORD_COUNT; i++)
    {
        Str t = token_entries[i].text;

        if (memcmp(t.data, *text, MIN(t.size, len)) == 0)
        {
            *text += t.size;
            return (Token) {
                .type = token_entries[i].type,
            };
        }
    }

    return (Token){};
}

static void skip_spaces(Str* text)
{
    while (isspace(*text->data))
    {
        ++text->data;
    }
}
