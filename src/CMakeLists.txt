add_subdirectory(lib)

add_executable(play_simple main_simple.cpp)
add_executable(play_para main_parallel.cpp)

target_link_libraries(play_simple PRIVATE params_lib)
target_link_libraries(play_simple PRIVATE array_lib)
target_link_libraries(play_simple PRIVATE grid_lib)
target_link_libraries(play_simple PRIVATE board_lib)
target_link_libraries(play_simple PRIVATE func_lib)

target_link_libraries(play_para PRIVATE params_lib)
target_link_libraries(play_para PRIVATE array_lib)
target_link_libraries(play_para PRIVATE grid_lib)
target_link_libraries(play_para PRIVATE board_lib)
target_link_libraries(play_para PRIVATE func_lib)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(play_para PUBLIC OpenMP::OpenMP_CXX)
endif()

find_package(MPI REQUIRED)
target_link_libraries(play_para PUBLIC MPI::MPI_CXX)
