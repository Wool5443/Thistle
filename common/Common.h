#ifndef THISTLE_EXITFRONT_H_
#define THISTLE_EXITFRONT_H_

#include <setjmp.h>
#include <stdnoreturn.h>

#include "Allocator.h"
#include "Error.h"

extern jmp_buf thistle_jmp_buf;
extern Allocator thistle_arena_allocator;

Error_code thistle_arena_allocator_init(size_t size);
void thistle_arena_flush();

noreturn void exit_thistle(int err);

#define THROW(error, ...)                   \
do                                          \
{                                           \
    int err = error;                        \
    log_error(__VA_ARGS__);                 \
    exit_thistle(err);                      \
} while (0)

#define TRY(expr) {auto e = (expr); if (e) THROW(e);}

#define TRY_RES(result)                     \
({                                          \
    auto t = (result);                      \
    if (t.error_code)                       \
        THROW(t.error_code);                \
    t.value;                                \
})

#endif // THISTLE_EXITFRONT_H_
