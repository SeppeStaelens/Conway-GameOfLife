add_library(board_lib
    STATIC 
    Board.hpp 
)

add_library(params_lib
    STATIC 
    GameParams.hpp 
)

set_target_properties(board_lib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(params_lib PROPERTIES LINKER_LANGUAGE CXX)
