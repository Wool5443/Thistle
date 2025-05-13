add_library(thistle_common src/Common.c)
target_include_directories(thistle_common PUBLIC src/)
target_link_libraries(thistle_common cmlib_vector cmlib_string)

add_library(thistle_tree src/Tree.c)
target_include_directories(thistle_tree PUBLIC src/)
target_link_libraries(thistle_tree thistle_common cmlib_string)
