cmake_minimum_required(VERSION 3.5)
set(projectName "Thistle")

project(${projectName} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS_DEBUG "-Wno-switch -Wno-conversion -Wno-unused-variable -Wno-pointer-arith -g -D _DEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Winit-self -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -march=native -mavx512f -O3 -DNDEBUG")

add_compile_definitions(LOGGING)

FILE(GLOB SOURCES_CPP src/*.cpp Utils/*.cpp)
FILE(GLOB SOURCES_HPP headers/*.hpp Utils/*.hpp libs/mlib/headers)

file(MAKE_DIRECTORY "listLog")
file(MAKE_DIRECTORY "listLog/txt")
file(MAKE_DIRECTORY "listLog/dot")
file(MAKE_DIRECTORY "listLog/img")

file(MAKE_DIRECTORY "treeLog")
file(MAKE_DIRECTORY "treeLog/dot")
file(MAKE_DIRECTORY "treeLog/img")

set(SOURCES
    ${SOURCES_CPP}
    ${SOURCES_HPP}
)

include_directories(headers Utils/ libs/mlib/headers)

add_executable(${projectName} ${SOURCES})

target_link_libraries(${projectName})
