#include <setjmp.h>

#include "String.h"
#include "Arena.h"
#include "Allocator.h"

Arena front_arena;

int main(int argc, const char* argv[])
{
    ERROR_CHECKING();

    logger_init_console();

    String source_file = {};

    Result_Arena front_arena_res = arena_ctor(1024 * 1024 * 100);
    CHECK_ERROR(front_arena_res.error_code);
    front_arena = front_arena_res.value;

    Result_String source_file_res = read_file(argv[1]);
    CHECK_ERROR(source_file_res.error_code);
    source_file = source_file_res.value;

    jmp_buf front_jmp_buf;

    switch (setjmp(front_jmp_buf))
    {
        case 0:
            longjmp(front_jmp_buf, 1);
        default:
            log_error("sex");
            ERROR_LEAVE();
    }

ERROR_CASE
    string_dtor(&source_file);
    free_arena(&front_arena);
}
