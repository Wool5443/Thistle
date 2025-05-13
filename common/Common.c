#include <stdlib.h>
#include "Common.h"
#include "Arena.h"

jmp_buf thistle_jmp_buf;
Allocator thistle_arena_allocator;

static Arena thistle_arena_;

static void* thistle_allocate(size_t size);
static void thistle_free(void*);

static void thistle_arena_clean_();

noreturn void exit_thistle(int err)
{
    longjmp(thistle_jmp_buf, err);
}

Error_code thistle_arena_allocator_init(size_t size)
{
    if (!thistle_arena_allocator.allocate)
    {
        Result_Arena arena = arena_ctor(size);
        if (arena.error_code) return arena.error_code;
        thistle_arena_ = arena.value;

        thistle_arena_allocator = (Allocator) {
            .allocate = thistle_allocate,
            .free     = thistle_free,
        };
        atexit(thistle_arena_clean_);
    }

    return EVERYTHING_FINE;
}

void thistle_arena_flush()
{
    arena_flush(&thistle_arena_);
}

static void* thistle_allocate(size_t size)
{
    void* p = arena_allocate(&thistle_arena_, size);
    return p;
}

static void thistle_free(void*) {}

static void thistle_arena_clean_()
{
    arena_dtor(&thistle_arena_);
}
