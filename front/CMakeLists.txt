set(SOURCES
    src/main.c
    src/Lexer.c
    src/Front.c
    src/RecursiveDescent.c
    src/Tokens.c
)

add_executable(front ${SOURCES})
target_link_libraries(front thistle_tree cmlib_string cmlib_vector)
