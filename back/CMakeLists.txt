set(NAME back)

set(SOURCES
    src/main.c
    src/Back.c
    src/Symbols.c
)

add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} thistle_tree cmlib_vector)
