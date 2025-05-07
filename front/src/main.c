#include <setjmp.h>

#include "Front.h"
#include "FrontCommon.h"

#include "Vector.h"
#include "String.h"

int main(int, const char* argv[])
{
    ERROR_CHECKING();

    // logger_init_console();
    logger_init_path("log.txt");

    String source_file = {};
    CHECK_ERROR(front_arena_allocator_init(1024 * 1024 * 500));

    Current_vector_allocator = &front_arena_allocator;
    Current_string_allocator = &front_arena_allocator;

    Result_String source_file_res = read_file(argv[1]);
    CHECK_ERROR(source_file_res.error_code);
    source_file = source_file_res.value;

    if (!(err = setjmp(front_jmp_buf)))
    {
        run_front(str_ctor_string(source_file));
    }
    else
    {
        ERROR_LEAVE();
    }

    return 0;

ERROR_CASE;
    return 1;
}
