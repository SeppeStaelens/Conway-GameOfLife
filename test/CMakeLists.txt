add_executable(test_functions
  test_functions.cpp
)

add_executable(test_board
  test_board.cpp
)

target_link_libraries(test_functions 
  PUBLIC 
  func_lib 
  GTest::gtest_main
)

target_link_libraries(test_board 
  PUBLIC 
  board_lib 
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_functions)
gtest_discover_tests(test_board)
