#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "lib/Board.hpp"
#include "lib/GameParams.hpp"
#include "lib/Functions.hpp"

int main(int argc, char* argv[]) {

    /*Create and read the parameters for this particular game.
      The parameter file is parsed as a command line argument.*/
    GameParams params;
    params.readParams(argv[1]);

    /* Parallelisation.*/
    omp_set_num_threads(params.num_threads);

    MPI_Init(&argc, &argv);
    int rank, nranks;
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (nranks % 2 == 1) {
      std::cout << "Please specify an even number of MPI ranks" << std::endl;
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* Display the parameters*/
    if (rank == 0){
      std::cout << std::endl << "Parameters for the Game of Life:" << std::endl << std::endl;
      params.display();
      std::cout << std::endl;
    }

    /* Create the Cartesian communicator.*/
    MPI_Comm cartesian2d;
    int dims[2] = {2, 2};
    int periods[2] = {1, 1};
    int reorder = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartesian2d);

    /* Store the coordinates of all the ranks on rank 0.*/
    int coords[2*nranks];
    int coord2d[2];

    if (rank == 0){
      for (int i = 0; i < nranks; i++){
        MPI_Cart_coords(cartesian2d, i, 2, coord2d);
        std::cout << "In 2D topology, Processor " << i << " has coordinates " << coord2d[0] << ", " << coord2d[1] << std::endl;
        coords[2*i] = coord2d[0];
        coords[2*i + 1] = coord2d[1];
      }
    }

    // Get the neighbours 1 step in each direction
    int left, right, up, down;
    MPI_Cart_shift(cartesian2d, 0, 1, &left, &right);
    MPI_Cart_shift(cartesian2d, 1, 1, &up, &down);

    std::cout << "In 2D topology, Processor " << rank << " has left/right neighbours " << left << " and " << right << std::endl;
    std::cout << "In 2D topology, Processor " << rank << " has up/down neighbours " << up << " and " << down << std::endl;

    /*Create the motherboard, i.e. the overarching grid on which we play, on rank 0.
      We assume it is square, and that all data can be read on one core.*/
    if (rank == 0){
      Grid motherboard(params.board_size, params.board_size, params.N_critical);

      if (params.random_data == 1) {
        initialize_random(&motherboard, &params);
      } else {
        initialize_from_file(&motherboard, &params, params.board_file);
      }

      /* From this motherboard, we will now send sub-boards to all the MPI ranks,
         depending on their Cartesian coordinates.*/

      Board board(&motherboard, params.board_size/2, params.board_size, 0, params.board_size/2);

      board.display();
      
      for (int i = 0; i < 9; i++){
        std::cout << board.data[i] << std::endl;
      }

      MPI_Send(&(board.data), board.size, MPI_INT, 1, 11, cartesian2d);
      
    }

    MPI_Status status;

    if (rank == 1){
      int message[9];

      for (int i = 0; i < 9; i++){
        std::cout << message[i] << std::endl;
      }

      MPI_Recv(&message, 9, MPI_INT, 0, 11, cartesian2d, &status);

      for (int i = 0; i < 9; i++){
        std::cout << message[i] << " x" << std::endl;
      }

      Array1D arr(9);

      for (int i = 0; i < 9; i++){
        arr(i) = message[i];
      }
      
      arr.display();

    }

    
    MPI_Finalize();
    
    return 0;
}
