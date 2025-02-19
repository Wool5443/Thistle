#ifndef THISTLE_EXITFRONT_H_
#define THISTLE_EXITFRONT_H_

#include <setjmp.h>
#include <stdnoreturn.h>

#include "Allocator.h"
#include "Error.h"

extern jmp_buf front_jmp_buf;
extern Allocator front_arena_allocator;

Error_code front_arena_allocator_init(size_t size);
void front_arena_flush();

INLINE noreturn void exit_front(int err)
{
    longjmp(front_jmp_buf, err);
}

#define THROW(error, ...)           \
do                                  \
{                                   \
    int err = error;                \
    log_error(__VA_ARGS__);         \
    exit_front(err);                \
} while (0)

#endif // THISTLE_EXITFRONT_H_
