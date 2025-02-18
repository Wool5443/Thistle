set(SOURCES
    src/main.c
)

add_executable(front ${SOURCES})
target_link_libraries(front cmlib_string)
