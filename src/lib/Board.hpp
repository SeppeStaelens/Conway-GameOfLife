#include <omp.h>

#include <cassert>
#include <fstream>
#include <iostream>

#include "Array1D.hpp"
#include "Grid.hpp"

#ifndef BOARD_HPP
#define BOARD_HPP

//!  A class inheriting from Grid, that adds the functionality to update the
//!  board.
class Board : public Grid {
 public:
  //! The ghost row at the bottom of the grid, including the corners
  Array1D bottom_ghost_row;
  //! The ghost row at the top of the grid, including the corners
  Array1D upper_ghost_row;
  //! The ghost column on the left side of the grid
  Array1D left_ghost_col;
  //! The ghost column on the right side of the grid
  Array1D right_ghost_col;

  //! Storage arrays to hold the horizontal neighbours counts in a row
  Array1D temp1, temp2, temp3;

  //! Constructor
  /*!
      \param N_row The number of rows in the grid
      \param N_col The number of columns in the grid
  */
  Board(int N_row, int N_col)
      : Grid(N_row, N_col),
        bottom_ghost_row(N_col + 2),
        upper_ghost_row(N_col + 2),
        left_ghost_col(N_row),
        right_ghost_col(N_row),
        temp1(N_col),
        temp2(N_col),
        temp3(N_col) {
    // Check if the grid is large enough to be sensible in the update procedure.
    assert(N_row > 2 && N_col > 2);
  }

  //! Initialize the board with values from (a subgrid of) the motherboard
  /*!
      \param motherboard The motherboard grid to copy values from
      \param row_low The lowest row index to copy from the motherboard
      \param col_left The leftmost column index to copy from the motherboard
  */
  void init_from_motherboard(Grid* motherboard, int row_low, int col_left) {
    N_nb_crit = (*motherboard).N_nb_crit;
#pragma omp parallel for collapse(2)
    for (int i = 0; i < N_row; ++i) {
      for (int j = 0; j < N_col; ++j) {
        data[i * N_col + j] = (*motherboard)(row_low + i, col_left + j);
      }
    }
  }

  //! Set the bottom ghost row based on an input array
  /*!
      \param target The array to copy values from
  */
  void set_bottom_ghost_row(Array1D* target) {
    assert(target->size == N_col + 2);
#pragma omp parallel for
    for (int i = 0; i < N_col + 2; ++i) {
      bottom_ghost_row(i) = (*target)(i);
    }
  }

  //! Set the upper ghost row based on an input array
  /*!
      \param target The array to copy values from
  */
  void set_upper_ghost_row(Array1D* target) {
    assert(target->size == N_col + 2);
#pragma omp parallel for
    for (int i = 0; i < N_col + 2; ++i) {
      upper_ghost_row(i) = (*target)(i);
    }
  }

  //! Set the left ghost column based on an input array
  /*!
      \param target The array to copy values from
  */
  void set_left_ghost_col(Array1D* target) {
    assert(target->size == N_row);
#pragma omp parallel for
    for (int i = 0; i < N_row; ++i) {
      left_ghost_col(i) = (*target)(i);
    }
  }

  //! Set the right ghost column based on an input array
  /*!
      \param target The array to copy values from
  */
  void set_right_ghost_col(Array1D* target) {
    assert(target->size == N_row);
#pragma omp parallel for
    for (int i = 0; i < N_row; ++i) {
      right_ghost_col(i) = (*target)(i);
    }
  }
  //! Store the neighbour counts of a row in an array
  /*!
      \param store The array to store the neighbour counts in
      \param n_row The row index to store the neighbour counts of
  */
  void store_neighbour_row(Array1D* store, int n_row) {
    (*store)(0) = left_ghost_col(n_row) + data[n_row * N_col + 0] +
                  data[n_row * N_col + 1];
#pragma omp parallel for
    for (int i = 1; i < N_col - 1; ++i) {
      (*store)(i) = data[n_row * N_col + i - 1] + data[n_row * N_col + i] +
                    data[n_row * N_col + i + 1];
    }
    (*store)(N_col - 1) = data[n_row * N_col + N_col - 2] +
                          data[n_row * N_col + N_col - 1] +
                          right_ghost_col(n_row);
  }

  //! Store the neighbour counts of the upper ghost row in an array
  /*!
      \param store The array to store the neighbour counts in
  */
  void store_upper_ghost_neighbour_row(Array1D* store) {
#pragma omp parallel for
    for (int i = 0; i < N_col; ++i) {
      (*store)(i) =
          upper_ghost_row(i) + upper_ghost_row(i + 1) + upper_ghost_row(i + 2);
    }
  }

  //! Store the neighbour counts of the bottom ghost row in an array
  /*!
      \param store The array to store the neighbour counts in
  */
  void store_bottom_ghost_neighbour_row(Array1D* store) {
#pragma omp parallel for
    for (int i = 0; i < N_col; ++i) {
      (*store)(i) = bottom_ghost_row(i) + bottom_ghost_row(i + 1) +
                    bottom_ghost_row(i + 2);
    }
  }

  //! Display the board, including the ghost rows and columns
  void ghost_display() {
    upper_ghost_row.display();
    for (int i = 0; i < N_row; ++i) {
      std::cout << left_ghost_col(i) << " ";
      for (int j = 0; j < N_col; ++j) {
        std::cout << data[i * N_col + j] << " ";
      }
      std::cout << right_ghost_col(i) << std::endl;
    }
    bottom_ghost_row.display();
  }

  //! Update the board based on the rules of the game of life
  void update_board() {
    // Storage
    int N_nb{0};
    int val{0};

    // Start with the top row, which requires the neighbours of the upper ghost
    // row
    store_upper_ghost_neighbour_row(&temp1);
    store_neighbour_row(&temp2, 0);
    store_neighbour_row(&temp3, 1);
#pragma omp parallel for
    for (int j = 0; j < N_col; ++j) {
      val = data[j];
      N_nb = temp1(j) + temp2(j) + temp3(j) - val;
      data[j] = (1 - val) * (N_nb == N_nb_crit) +
                val * (N_nb == N_nb_crit || N_nb == N_nb_crit - 1);
    }

    // Then the middle rows
    for (int i = 1; i < N_row - 1; ++i) {
      temp1.copy_into(&temp2);
      temp2.copy_into(&temp3);
      store_neighbour_row(&temp3, i + 1);
#pragma omp parallel for
      for (int j = 0; j < N_col; ++j) {
        val = data[i * N_col + j];
        N_nb = temp1(j) + temp2(j) + temp3(j) - val;
        data[i * N_col + j] =
            (1 - val) * (N_nb == N_nb_crit) +
            val * (N_nb == N_nb_crit || N_nb == N_nb_crit - 1);
      }
    }

    // Finally the bottom row, which requires the neighbours of the bottom ghost
    // row
    temp1.copy_into(&temp2);
    temp2.copy_into(&temp3);
    store_bottom_ghost_neighbour_row(&temp3);
#pragma omp parallel for
    for (int j = 0; j < N_col; ++j) {
      val = data[(N_row - 1) * N_col + j];
      N_nb = temp1(j) + temp2(j) + temp3(j) - val;
      data[(N_row - 1) * N_col + j] =
          (1 - val) * (N_nb == N_nb_crit) +
          val * (N_nb == N_nb_crit || N_nb == N_nb_crit - 1);
    }
  }
};

#endif
