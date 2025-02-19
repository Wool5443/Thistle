set(SOURCES
    src/main.c
    src/Lexer.c
)

add_executable(front ${SOURCES})
target_link_libraries(front cmlib_string cmlib_vector)
