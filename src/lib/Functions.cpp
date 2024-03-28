#include "Functions.hpp"

#include <omp.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "Array1D.hpp"
#include "Board.hpp"
#include "GameParams.hpp"
#include "Grid.hpp"

//! Initialize the board with random data
/*!
    \param grid The grid to be initialized
    \param params The parameters for the game
*/
void functions::initialize_random(Grid* grid, GameParams* params) {
  // Use a random device to seed the random number engine
  std::random_device rd;
  std::mt19937 gen(rd());

  /* Define the Bernoulli distribution for generating random binary numbers (0
     or 1). The parameter params.prob_live reflects the probability of
     generating a 1.*/
  std::bernoulli_distribution dist((*params).prob_live);

// Fill the matrix with random binary numbers
#pragma omp parallel for collapse(2)
  for (int i = 0; i < (*grid).N_row; ++i) {
    for (int j = 0; j < (*grid).N_col; ++j) {
      (*grid)(i, j) = dist(gen);
    }
  }

  std::cout << "Board initialized randomly." << std::endl << std::endl;
}

//! Initialize the board from a file
/*!
    \param grid The grid to be initialized
    \param params The parameters for the game
    \param file The file to read the data from
*/
void functions::initialize_from_file(Grid* grid, GameParams* params,
                                     std::string file) {
  std::ifstream inputFile(file);

  if (!inputFile) {
    std::cerr << "Error opening file!" << std::endl;
  }

  std::string line;
  int i = 0;
  int j = 0;

  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);

    int bit;
    while (iss >> bit) {
      (*grid)(i, j) = bit;
      j += 1;
    }
    j = 0;
    i += 1;
  }
  std::cout << "Board initialized from " << file << std::endl << std::endl;
}

//! Update the board for a given number of steps
/*!
    \param board The board to be updated
    \param params The parameters for the game, including the number of evolve
   steps \param store_row An array to store ghost rows \param store_col An array
   to store ghost columns
*/
void functions::iteration_one_board(Board* board, GameParams* params,
                                    Array1D* store_row, Array1D* store_col) {
  std::string path = (*params).output_path;

  std::cout << "Starting updating iteration." << std::endl;

  for (int i = 1; i <= (*params).evolve_steps; i++) {
    (*board).update_board();

    (*store_col)
        .overwrite((*board).sub_col((*board).N_col - 1, 0, (*board).N_row));

    (*board).set_left_ghost_col(store_col);
    (*store_col).overwrite((*board).sub_col(0, 0, (*board).N_row));
    (*board).set_right_ghost_col(store_col);

    (*store_row).overwrite((*board).periodic_row((*board).N_row - 1));
    (*board).set_upper_ghost_row(store_row);
    (*store_row).overwrite((*board).periodic_row(0));
    (*board).set_bottom_ghost_row(store_row);

    if (i % (*params).save_interval == 0) {
      path = (*params).output_path + "step" + std::to_string(i) + ".txt";
      (*board).save(path);
    }
  }
}

//! Find the largest divisor d of a number n that is smaller than sqrt(n) and an
//! upper bound
/*!
    \param n The number to find the divisor of
    \param upper_bound The upper bound for the divisor
    \return The largest divisor of n that is smaller than sqrt(n)
*/
int functions::find_largest_divisor(int n, int upper_bound) {
  double s = sqrt(n);
  int d = std::min(upper_bound, static_cast<int>(floor(s)));
  while (n % d != 0) {
    d--;
  }
  return d;
}

//! Find the dimensions of the Cartesian grid communicator, if possible.
/*!
    \param board_size The size of the board
    \param nranks The number of ranks
    \return A tuple with the dimensions of the Cartesian grid communicator
*/
std::tuple<int, int> functions::find_Cart_dim(int board_size, int nranks) {
  int d1 = functions::find_largest_divisor(nranks, nranks);
  int d2 = nranks / d1;

  while ((board_size % d1 != 0 || board_size % d2 != 0) && d1 > 1) {
    d1 = functions::find_largest_divisor(nranks, d1 - 1);
    d2 = nranks / d1;
  }

  if (d1 == 1) {
    if (board_size % d2 == 0 && board_size > nranks) {
      return std::make_tuple(1, d2);
    } else {
      std::cout << "\n\n";
      std::cerr << "\nError: Could not find suitable grid parameters."
                << std::endl;
      std::cout << "Try again and make sure nranks = a x b with a,b divisors "
                   "of the board size.\n";
      exit(1);
    }
  } else {
    return std::make_tuple(d1, d2);
  }
}
