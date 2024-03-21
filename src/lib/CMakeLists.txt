add_library(board_lib
    STATIC 
    Board.hpp 
)

add_library(params_lib
    STATIC 
    GameParams.hpp 
)

add_library(func_lib
    STATIC 
    Functions.cpp
    Functions.hpp 
)

set_target_properties(board_lib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(params_lib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(func_lib PROPERTIES LINKER_LANGUAGE CXX)
