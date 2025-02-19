#include <setjmp.h>

#include "String.h"
#include "Front.h"
#include "Lexer.h"
#include "Arena.h"

void noreturn exit_front(int err);

jmp_buf front_jmp_buf;

Allocator front_arena_allocator;

Arena front_arena = {};

void* front_allocate(size_t size);
void front_free(void*);

int main(int, const char* argv[])
{
    ERROR_CHECKING();

    logger_init_console();

    String source_file = {};

    Result_Arena front_arena_res = arena_ctor(1024 * 1024 * 1024);
    CHECK_ERROR(front_arena_res.error_code);
    front_arena = front_arena_res.value;

    front_arena_allocator = (Allocator) {
        front_allocate,
        front_free,
    };

    Current_vector_allocator = front_arena_allocator;
    Current_string_allocator = front_arena_allocator;

    Result_String source_file_res = read_file(argv[1]);
    CHECK_ERROR(source_file_res.error_code);
    source_file = source_file_res.value;

    switch (setjmp(front_jmp_buf))
    {
        case 0:
            Tokens tokens = tokenize(str_ctor_string(source_file));
            for (size_t i = 0, end = vec_size(tokens); i < end; i++)
            {
                printf("%d\n", tokens[i].type);
            }
            break;
        default:
            log_error("sex");
            ERROR_LEAVE();
    }

ERROR_CASE
    string_dtor(&source_file);
    free_arena(&front_arena);
}

void* front_allocate(size_t size)
{
    return arena_allocate(&front_arena, size);
}

void front_free(void*) {}
