#include "Lexer.h"

#include <ctype.h>
#include <math.h>

#include "Common.h"
#include "DSL.h"

static Tokens tokenize_word(Str word);
static Str* split(Str text);
static Token read_string(const char** text);
static Token read_name(const char** text);
static Token read_number(const char** text);
static Token read_keyword(const char** text);
static Token read_end(const char** text);

static bool is_name(char c);

void delete_comments(String text)
{
    static const char* COMMENT = "//";

    char* com_ptr = NULL;
    while ((com_ptr = strstr(text.data, COMMENT)))
    {
        const char* com_end = strchr(com_ptr, '\n');
        if (!com_end)
        {
            THROW(ERROR_SYNTAX, "\n not found in comment");
        }
        memset(com_ptr, ' ', com_end - com_ptr);
    }
}

Tokens tokenize(Str text)
{
    Str* words = split(text);
    Tokens tokens = {};

    VEC_ITER(words, i)
    {
        Tokens ts = tokenize_word(words[i]);
        VEC_ITER(ts, j)
        {
            TRY(vec_add(tokens, ts[j]));
        }
    }
    TRY(vec_add(tokens, T(TOK_END)));

    return tokens;
}

static Tokens tokenize_word(Str word)
{
    Tokens tokens = {};

    const char* p = word.data;
    const char* end = p + word.size;

    while (p < end)
    {

#define ATTEMPT_READ(f)                                                        \
    ({                                                                         \
        Token val = f(&p);                                                     \
        if (val.type != TOK_BAD)                                               \
        {                                                                      \
            TRY(vec_add(tokens, val));                                         \
            continue;                                                          \
        }                                                                      \
    })

        ATTEMPT_READ(read_end);
        ATTEMPT_READ(read_string);
        ATTEMPT_READ(read_keyword);
        ATTEMPT_READ(read_number);
        ATTEMPT_READ(read_name);

#undef ATTEMPT_READ
    }

    return tokens;
}

static Str* split(Str text)
{
    static const char* WHITE_SPACE = " \n\r\t\v\f\"";

    Str* words = NULL;
    const char* prev = text.data;
    const char* curr = NULL;

    while ((curr = strpbrk(prev, WHITE_SPACE)))
    {
        TRY(vec_add(words, str_ctor_size(prev, curr - prev)));
        if (*curr == '\"')
        {
            const char* close_mark = strchr(curr + 1, '\"');
            if (!close_mark)
            {
                THROW(ERROR_SYNTAX, "Matching '\"' expected");
            }
            TRY(vec_add(words, str_ctor_size(curr, close_mark - curr + 1)));
            curr = close_mark;
        }
        prev = curr + 1;
    }

    return words;
}

static Token read_string(const char** text)
{
    assert(text);

    const char* ptr = *text;

    if (*ptr != '"')
    {
        return ET;
    }
    ptr++;

    String string = TRY_RES(string_ctor_capacity(1));

    while (*ptr && (*ptr != '"' || (*(ptr - 1) == '\\' && *ptr == '"')))
    {
        TRY(string_append_char(&string, *ptr++));
    }

    if (*ptr != '"')
    {
        THROW(ERROR_SYNTAX, "String literal did not end with \"");
    }
    ptr++;

    *text = ptr;

    return (Token) {
        .type = TOK_STRING,
        .string = string,
    };
}

static Token read_name(const char** text)
{
    assert(text);

    const char* ptr = *text;

    String name = TRY_RES(string_ctor_capacity(1));
    if (isalpha(*ptr) || *ptr == '_')
    {
        TRY(string_append_char(&name, *ptr++));
    }

    while (is_name(*ptr))
    {
        TRY(string_append_char(&name, *ptr++));
    }

    *text = ptr;

    return (Token) {
        .type = TOK_NAME,
        .string = name,
    };
}

static Token read_number(const char** text)
{
    assert(text);

    char* endp = NULL;
    int64_t integer = strtoll(*text, &endp, 0);
    double floating = NAN;

    if (*endp == '.')
    {
        floating = strtod(*text, &endp);
        if (!isfinite(floating))
        {
            return ET;
        }
    }
    else if (integer == 0
             && ((endp == *text) || (*endp != '\0' && !isspace(*endp))))
    {
        return ET;
    }

    *text = endp;

    if (isfinite(floating))
    {
        return (Token) {
            .type = TOK_FLOAT,
            .floating = floating,
        };
    }

    return (Token) {
        .type = TOK_INTEGER,
        .integer = integer,
    };
}

static Token read_keyword(const char** text)
{
    assert(text);

    size_t len = strlen(*text);

    for (int i = 1; i < KEYWORD_COUNT; i++)
    {
        Str t = token_literal((Token_type)i);

        if (memcmp(t.data, *text, MIN(t.size, len)) == 0)
        {
            *text += t.size;
            return T(i);
        }
    }

    return ET;
}

static Token read_end(const char** text)
{
    if (**text == '\0')
        return T(TOK_END);
    return T(TOK_BAD);
}

static bool is_name(char c)
{
    return isalnum(c) || c == '_';
}
