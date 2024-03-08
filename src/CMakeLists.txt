add_subdirectory(lib)

add_executable(play main.cpp)

target_link_libraries(play PRIVATE params_lib)
target_link_libraries(play PRIVATE board_lib)
