add_subdirectory(lib)

add_executable(play_simple main_simple.cpp)

target_link_libraries(play_simple PRIVATE params_lib)
target_link_libraries(play_simple PRIVATE board_lib)
