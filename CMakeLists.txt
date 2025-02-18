cmake_minimum_required(VERSION 3.30)
project(Thistle C)

# add_compile_definitions("DISABLE_LOGGING")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

set(CMAKE_C_FLAGS_DEBUG "\
-pthread -g -D _DEBUG -O0 -Wall -Wextra -fPIC \
-fsanitize=address,undefined,leak,shift,float-divide-by-zero,signed-integer-overflow\
")
set(CMAKE_CXX_FLAGS_RELEASE "-pthread -O3 -g -fPIC -DNDEBUG -march=native")

add_subdirectory(cmlib)
add_subdirectory(front)
