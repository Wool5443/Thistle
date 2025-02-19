set(SOURCES
    src/main.c
    src/Lexer.c
    src/Front.c
    src/FrontCommon.c
    src/Tree.c
    src/RecursiveDescent.c
)

add_executable(front ${SOURCES})
target_link_libraries(front cmlib_string cmlib_vector)
