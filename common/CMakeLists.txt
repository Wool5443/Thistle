add_library(thistle_common Common.c)
target_include_directories(thistle_common PUBLIC .)
target_link_libraries(thistle_common cmlib_allocator cmlib_logger)

add_library(thistle_tree Tree.c)
target_include_directories(thistle_tree PUBLIC .)
target_link_libraries(thistle_tree thistle_common cmlib_string)
