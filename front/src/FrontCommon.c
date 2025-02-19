#include "FrontCommon.h"
#include "Arena.h"

jmp_buf front_jmp_buf;

void noreturn exit_front(int err);

Arena front_arena_;
Allocator front_arena_allocator;

void* front_allocate(size_t size);
void front_free(void*);

static void front_arena_clean_();

Error_code front_arena_allocator_init(size_t size)
{
    if (!front_arena_allocator.allocate)
    {
        Result_Arena arena = arena_ctor(size);
        if (arena.error_code) return arena.error_code;
        front_arena_ = arena.value;

        front_arena_allocator = (Allocator) {
            .allocate = front_allocate,
            .free     = front_free,
        };
        atexit(front_arena_clean_);
    }

    return EVERYTHING_FINE;
}

void front_arena_flush()
{
    arena_flush(&front_arena_);
}

void* front_allocate(size_t size)
{
    return arena_allocate(&front_arena_, size);
}

void front_free(void*) {}

static void front_arena_clean_()
{
    arena_dtor(&front_arena_);
}
