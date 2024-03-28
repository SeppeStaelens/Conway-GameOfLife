#include <mpi.h>
#include <omp.h>

#include <cassert>
#include <iostream>
#include <tuple>

// Below needed for correct rendering on my laptop
//#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"
#include "lib/Array1D.hpp"
#include "lib/Board.hpp"
#include "lib/Functions.hpp"
#include "lib/GameParams.hpp"
#include "lib/Grid.hpp"

// #define DEBUG

int main(int argc, char *argv[]) {
  /*
  Create and read the parameters for this particular game.
    The parameter file is parsed as a command line argument.
    */
  GameParams params;
  params.readParams(argv[1]);

  /*
  Parallelisation.
  */
  omp_set_num_threads(params.num_threads);

  MPI_Init(&argc, &argv);
  int rank, nranks;
  MPI_Comm_size(MPI_COMM_WORLD, &nranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /*
  Determine the dimensions of the Cartesian communicator
  */
  int d1, d2;

  if (rank == 0) {
    std::tie(d1, d2) = functions::find_Cart_dim(params.board_size, nranks);

    std::cout << "\nNumber of OMP threads: " << params.num_threads << std::endl;
    std::cout << "Number of MPI ranks: " << nranks << " = " << d1 << " x " << d2
              << std::endl;

    // Display the parameters
    std::cout << std::endl
              << "Parameters for the Game of Life:" << std::endl
              << std::endl;
    params.display();
    std::cout << std::endl;
  }

  MPI_Bcast(&d1, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&d2, 1, MPI_INT, 0, MPI_COMM_WORLD);

  /*
  Create the Cartesian communicator.
  */
  MPI_Comm cartesian2d;
  int dims[2] = {d1, d2};
  int periods[2] = {1, 1};
  int reorder = 1;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartesian2d);

  /*
  Storing the coordinates of all the ranks.
  */
  int coords[2 * nranks];
  int coord2d[2];

  // Store all on rank 0
  if (rank == 0) {
    for (int i = 0; i < nranks; i++) {
      MPI_Cart_coords(cartesian2d, i, 2, coord2d);
#ifdef DEBUG
      std::cout << "In 2D topology, Processor " << i << " has coordinates "
                << coord2d[0] << ", " << coord2d[1] << std::endl;
#endif
      coords[2 * i] = coord2d[0];
      coords[2 * i + 1] = coord2d[1];
    }
  }

  // Store the coordinates of the current rank.
  MPI_Cart_coords(cartesian2d, rank, 2, coord2d);
  int co_x = coord2d[0];
  int co_y = coord2d[1];

  // Get the neighbours 1 step in each direction
  int left, right, up, down;
  MPI_Cart_shift(cartesian2d, 0, 1, &left, &right);
  MPI_Cart_shift(cartesian2d, 1, 1, &up, &down);

#ifdef DEBUG
  std::cout << "In 2D topology, Processor " << rank
            << " has left/right neighbours " << left << " and " << right
            << std::endl;
  std::cout << "In 2D topology, Processor " << rank
            << " has up/down neighbours " << up << " and " << down << std::endl;
#endif

  /*
  Create the motherboard, i.e. the overarching grid on which we play.
  We assume it is square, and that all data can be read on one core.
  We initialize it on rank 0.
  */

  Grid motherboard(params.board_size, params.board_size, params.N_critical);
  std::string path;

  if (rank == 0) {
    if (params.random_data == 1) {
      functions::initialize_random(&motherboard, &params);
    } else {
      functions::initialize_from_file(&motherboard, &params, params.board_file);
    }
    path = params.output_path + "step0.txt";
    motherboard.save(path);

#ifdef DEBUG
    motherboard.display();
    std::cout << std::endl;
#endif
  }

  /*
  Now we broadcast this motherboard to the other ranks.
  */

  MPI_Bcast(motherboard.data, motherboard.size, MPI_INT, 0, cartesian2d);

  int board_size_x = params.board_size / d1;
  int board_size_y = params.board_size / d2;

  if (rank == 0) {
    std::cout << "Board size induced by the " << d1 << " x " << d2
              << " Cartesian communicator: " << board_size_x << " x "
              << board_size_y << std::endl;
  }

  /*
  Now we need to select the correct sub-board at every rank.
  */
  Board board(board_size_y, board_size_x);
  board.init_from_motherboard(&motherboard, co_y * board_size_y,
                              co_x * board_size_x);

  /*
  Given the periodic boundary conditions, we need to set the ghost
  rows/columns. This looks involved, but the expressions are simply lengthy
  due to the coordinates induced by the Cartesian communicator.
  */
  Array1D col = motherboard.sub_col(
      (params.board_size + co_x * board_size_x - 1) % params.board_size,
      co_y * board_size_y, (co_y + 1) * board_size_y);
  board.set_left_ghost_col(&col);

  col.overwrite(
      motherboard.sub_col(((co_x + 1) * board_size_x) % params.board_size,
                          co_y * board_size_y, (co_y + 1) * board_size_y));
  board.set_right_ghost_col(&col);

  Array1D row(board.N_col + 2);
  row.overwrite(
      motherboard.sub_row(
          (params.board_size + co_y * board_size_y - 1) % params.board_size,
          co_x * board_size_x, (co_x + 1) * board_size_x),
      1);
  row(0) = motherboard(
      (params.board_size + co_y * board_size_y - 1) % params.board_size,
      (params.board_size + co_x * board_size_x - 1) % params.board_size);
  row(board.N_col + 1) = motherboard(
      (params.board_size + co_y * board_size_y - 1) % params.board_size,
      ((co_x + 1) * board_size_x + 1) % params.board_size);
  board.set_upper_ghost_row(&row);

  row.overwrite(
      motherboard.sub_row(((co_y + 1) * board_size_y) % params.board_size,
                          co_x * board_size_x, (co_x + 1) * board_size_x),
      1);
  row(0) = motherboard(
      ((co_y + 1) * board_size_y) % params.board_size,
      (params.board_size + co_x * board_size_x - 1) % params.board_size);
  row(board.N_col + 1) =
      motherboard(((co_y + 1) * board_size_y) % params.board_size,
                  ((co_x + 1) * board_size_x + 1) % params.board_size);
  board.set_bottom_ghost_row(&row);

#ifdef DEBUG
  if (rank == 0 || rank == 2) {
    /* Display the board with ghost cells attached.*/
    std::cout << "Processor " << rank << " with co " << co_x << ", " << co_y
              << " has ghost board " << std::endl;
    board.ghost_display();
    std::cout << std::endl;
  }
#endif

  /*
  Create the buffers for the communication.
  */
  Array1D bottom_row_p(board.N_col + 2);
  Array1D top_row_p(board.N_col + 2);
  Array1D left_col(board.N_row);
  Array1D right_col(board.N_row);

  MPI_Request req1, req2, req3, req4;

  /*
  Create storage for boards that rank 0 will receive.
  */
  int store_board[board.size];
  int r_co_x, r_co_y;
  MPI_Status status;

  for (int i = 1; i < params.evolve_steps + 1; i++) {
#ifdef DEBUG
    if (rank == 0 || rank == 2) {
      std::cout << "Rank " << rank << " is at step " << i << std::endl;
    }
#endif

    /* Update board.*/
    board.update_board();

    /* Store the edges of the board*/
    board.store_row(&bottom_row_p, board.N_row - 1, 1);
    board.store_row(&top_row_p, 0, 1);
    board.store_col(&left_col, 0);
    board.store_col(&right_col, board.N_col - 1);

    /* First we send around the (ghost) columns.*/
    MPI_Isend(left_col.data, board.N_row, MPI_INT, left, 13, cartesian2d,
              &req1);
    MPI_Isend(right_col.data, board.N_row, MPI_INT, right, 14, cartesian2d,
              &req2);

    MPI_Irecv(board.right_ghost_col.data, board.N_row, MPI_INT, right, 13,
              cartesian2d, &req1);
    MPI_Irecv(board.left_ghost_col.data, board.N_row, MPI_INT, left, 14,
              cartesian2d, &req2);

    MPI_Barrier(cartesian2d);

    /* We now use the NEW ghost columns to send the ghost CORNERS along with the
     * rows.*/
    bottom_row_p(0) = board.left_ghost_col(board.N_row - 1);
    bottom_row_p(board.N_col + 1) = board.right_ghost_col(board.N_row - 1);

    top_row_p(0) = board.left_ghost_col(0);
    top_row_p(board.N_col + 1) = board.right_ghost_col(0);

    /* Now we send around the ghost rows as well.*/
    MPI_Isend(bottom_row_p.data, board.N_col + 2, MPI_INT, down, 11,
              cartesian2d, &req3);
    MPI_Isend(top_row_p.data, board.N_col + 2, MPI_INT, up, 12, cartesian2d,
              &req4);

    MPI_Irecv(board.upper_ghost_row.data, board.N_col + 2, MPI_INT, up, 11,
              cartesian2d, &req3);
    MPI_Irecv(board.bottom_ghost_row.data, board.N_col + 2, MPI_INT, down, 12,
              cartesian2d, &req4);

    MPI_Barrier(cartesian2d);

    /*
    If we need to save the grid, communicate everything to rank 0.
    */
    if (i % params.save_interval == 0) {
      if (rank != 0) {
        MPI_Send(board.data, board.size, MPI_INT, 0, 20, cartesian2d);

#ifdef DEBUG
        std::cout << "Rank " << rank << " sent " << std::endl;
        board.display();
        std::cout << std::endl;
#endif
      }

      if (rank == 0) {
        motherboard.overwrite_sub_board(
            board.data, co_y * board_size_y, (co_y + 1) * board_size_y,
            co_x * board_size_x, (co_x + 1) * board_size_x);

#ifdef DEBUG
        std::cout << "From " << rank << " received" << std::endl;
        board.display();
        std::cout << std::endl;
#endif

        for (int i = 1; i < nranks; i++) {
          MPI_Recv(&store_board, board.size, MPI_INT, i, 20, cartesian2d,
                   &status);
          r_co_x = coords[2 * i];
          r_co_y = coords[2 * i + 1];
          motherboard.overwrite_sub_board(
              store_board, r_co_y * board_size_y, (r_co_y + 1) * board_size_y,
              r_co_x * board_size_x, (r_co_x + 1) * board_size_x);
        }

        path = params.output_path + "step" + std::to_string(i) + ".txt";
        motherboard.save(path);
      }
    }
  }

  MPI_Finalize();

  return 0;
}
