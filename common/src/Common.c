#include "Common.h"

#include <stdlib.h>

#include "Logger.h"

jmp_buf thistle_jmp_buf;
ArenaResource* thistle_arena_resource;

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

    Result_ArenaResource arena_res = arena_resource_ctor(1024 * 1024 * 50);
    CHECK_ERROR(arena_res.error_code);
    thistle_arena_resource = &arena_res.value;

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
