#include <setjmp.h>

#include "Front.h"
#include "Common.h"

#include "Vector.h"
#include "String.h"

int main(int argc, const char* argv[])
{
    ERROR_CHECKING();

    if (argc != 3)
    {
        printf("Usage: %s <source_file> <output_file>", argv[0]);
        return 1;
    }

    logger_init_path("log_front.txt");

    String source_file = {};
    CHECK_ERROR(thistle_arena_allocator_init(1024 * 1024 * 500));

    Current_vector_allocator = thistle_arena_allocator;
    Current_string_allocator = thistle_arena_allocator;

    Result_String source_file_res = read_file(argv[1]);
    CHECK_ERROR(source_file_res.error_code);
    source_file = source_file_res.value;

    if (!(err = setjmp(thistle_jmp_buf)))
    {
        run_front(source_file, str_ctor(argv[2]));
    }
    else
    {
        ERROR_LEAVE();
    }

    return 0;

ERROR_CASE;
    string_dtor(&source_file);
    return 1;
}
