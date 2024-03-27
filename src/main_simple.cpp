#include <iostream>

#include "lib/Array1D.hpp"
#include "lib/Board.hpp"
#include "lib/Functions.hpp"
#include "lib/GameParams.hpp"
#include "lib/Grid.hpp"

int main(int argc, char* argv[]) {
  /*Create and read the parameters for this particular game.
    The parameter file is parsed as a command line argument.*/
  GameParams params;
  params.readParams(argv[1]);
  std::cout << std::endl
            << "Parameters for the Game of Life:" << std::endl
            << std::endl;
  params.display();
  std::cout << std::endl;

  /*Create the motherboard, i.e. the overarching grid on which we play.
    We assume it is square, and that all data can be read on one core.
    For this simple program, we don't do domain decomposition, and therefore
    initialize a board class from the start.*/
  // Board board(params.board_size, params.board_size);
  Board board(params.board_size, params.board_size);
  board.N_nb_crit = params.N_critical;

  if (params.random_data == 1) {
    functions::initialize_random(&board, &params);
  } else {
    functions::initialize_from_file(&board, &params, params.board_file);
  }

  // Display the motherboard.
  if (params.board_size <= 10) {
    std::cout << "The motherboard." << std::endl << std::endl;
    board.display();
    std::cout << std::endl;
  }

  /* Save the initial state.*/
  std::string save_path = params.output_path + "step0.txt";
  board.save(save_path);

  /* Given the periodic boundary conditions, we need to set the ghost rows.
     The ghost columns are easy, they are simply the first and last column.*/
  Array1D col = board.sub_col(board.N_col - 1, 0, board.N_row);
  board.set_left_ghost_col(&col);

  col.overwrite(board.sub_col(0, 0, board.N_row));
  board.set_right_ghost_col(&col);

  /* For the ghost rows, we need an extra step, as the ghost corners are not
     trivially included in the upper and bottom row. This is wrapped in the
     periodic_row method.*/
  Array1D row = board.periodic_row(board.N_row - 1);
  board.set_upper_ghost_row(&row);

  row.overwrite(board.periodic_row(0));
  board.set_bottom_ghost_row(&row);

  if (params.board_size <= 10) {
    /* Display the board with ghost cells attached.*/
    std::cout << "Motherboard with ghost cells." << std::endl << std::endl;

    board.ghost_display();
  }

  /* Actual iteration of the update procedure.
     row and col are passed as fixed storage containers.*/
  functions::iteration_one_board(&board, &params, &row, &col);

  return 0;
}
