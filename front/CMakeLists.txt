set(NAME front)

set(SOURCES
    src/main.c
    src/Lexer.c
    src/Front.c
    src/RecursiveDescent.c
    src/Tokens.c
)

add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} thistle_tree cmlib_vector)
