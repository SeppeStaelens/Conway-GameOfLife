add_executable(test_functions
  test_functions.cpp
)

target_link_libraries(test_functions 
  PUBLIC 
  func_lib 
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_functions)
