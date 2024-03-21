#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "Board.hpp"
#include "GameParams.hpp"


void initialize_random(Grid* grid, GameParams* params);

void initialize_from_file(Grid* grid, GameParams* params, std::string file);

void iteration_one_board(Board* board, GameParams* params, Array1D* store_row, Array1D* store_col);









#endif
