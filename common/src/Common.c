#include <stdlib.h>
#include "Common.h"
#include "Arena.h"
#include "String.h"
#include "Vector.h"

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

int thistle_main(int argc, const char* argv[], thistle_func func)
{
    ERROR_CHECKING();

    if (argc != 3)
    {
        printf("Usage: %s <source_file> <output_file>", argv[0]);
        return 1;
    }

    logger_init_path("log_front.txt");

    CHECK_ERROR(thistle_arena_allocator_init(1024 * 1024 * 500));

    Current_vector_allocator = thistle_arena_allocator;
    Current_string_allocator = thistle_arena_allocator;

    if (!(err = setjmp(thistle_jmp_buf)))
    {
        func(argv[1], argv[2]);
    }
    else
    {
        ERROR_LEAVE();
    }

    return 0;

ERROR_CASE;
    return 1;
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
