cmake_minimum_required(VERSION 3.30)
project(Thistle C)

# add_compile_definitions("DISABLE_LOGGING")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)


set(CMAKE_C_FLAGS_DEBUG
"-march=native -g -D_DEBUG -O0 -Wall -Wextra -Werror -Waggressive-loop-optimizations \
-Wcast-align -Wchar-subscripts -Wempty-body \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wopenmp-simd -Wpacked -Winit-self \
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
-Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wsync-nand \
-Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
-Wno-missing-field-initializers -Wno-narrowing -Wstack-protector -fcheck-new \
-fstack-protector -fno-omit-frame-pointer -pie -fPIE -Werror=vla \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr"
)

set(CMAKE_C_FLAGS_RELEASE "-O3 -g -fPIC -DNDEBUG -march=native")

add_subdirectory(cmlib)
add_subdirectory(common)
add_subdirectory(front)
add_subdirectory(back)
