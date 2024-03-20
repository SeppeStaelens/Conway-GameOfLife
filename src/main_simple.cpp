#include <iostream>
#include <random>

#include "lib/Board.hpp"
#include "lib/GameParams.hpp"

int main(int argc, char* argv[]) {

    /*Create and read the parameters for this particular game.
      The parameter file is parsed as a command line argument.*/
    GameParams params;
    params.readParams(argv[1]);
    std::cout << std::endl << "Parameters for the Game of Life:" << std::endl << std::endl;
    params.display();
    std::cout << std::endl;
    
    /*Create the motherboard, i.e. the overarching grid on which we play.
      We assume it is square, and that all data can be read on one core.*/
    Grid motherboard(params.board_size, params.board_size);

    if (params.random_data == 1) {

        // Use a random device to seed the random number engine
        std::random_device rd;
        std::mt19937 gen(rd());
        
        /* Define the Bernoulli distribution for generating random binary numbers (0 or 1).
           The parameter params.prob_live reflects the probability of generating a 1.*/
        std::bernoulli_distribution dist(params.prob_live);

        // Fill the matrix with random binary numbers
        for (int i = 0; i < params.board_size; ++i) {
            for (int j = 0; j < params.board_size; ++j) {
                motherboard(i,j) = dist(gen);
            }
        }
    }

    // Display the motherboard.
    std::cout << "The motherboard." << std::endl << std::endl;
    motherboard.display();
    std::cout << std::endl; 

    /* For this simple example, we will not do domain decomposition. 
       Therefore, the entire Grid is passed to a motherboard class. */
    Board board(&motherboard, 0, params.board_size, 0, params.board_size);
    std::string save_path = params.output_path + "step0.txt";
    board.save(save_path);

    /* Given the periodic boundary conditions, we need to set the ghost rows.
       The ghost columns are easy, they are simply the first and last column.*/
    Array1D col = motherboard.sub_col(params.board_size - 1, 0, params.board_size);
    board.set_left_ghost_col(&col);

    col.overwrite(motherboard.sub_col(0, 0, params.board_size));
    board.set_right_ghost_col(&col);

    /* For the ghost rows, we need an extra step, as the ghost corners are not 
       trivially included in the upper and bottom row. This is wrapped in the periodic_row method.*/
    Array1D row = motherboard.periodic_row(params.board_size - 1);
    board.set_upper_ghost_row(&row);

    row.overwrite(motherboard.periodic_row(0));
    board.set_bottom_ghost_row(&row);

    /* Display the board with ghost cells attached.*/
    std::cout << "Motherboard with ghost cells." << std::endl << std::endl;

    board.ghost_display();
    
    std::cout << std::endl << "After one update." << std::endl;

    board.update_board();
    board.display();


    Array1D testarr =  board.sub_col(params.board_size - 1, 0, params.board_size);
    col.overwrite(board.sub_col(params.board_size - 1, 0, params.board_size));
    board.set_left_ghost_col(&col);
    col.overwrite(board.sub_col(0, 0, params.board_size));
    board.set_right_ghost_col(&col);

    row.overwrite(board.periodic_row(params.board_size - 1));
    board.set_upper_ghost_row(&row);
    row.overwrite(board.periodic_row(0));
    board.set_bottom_ghost_row(&row);

    std::cout << "Board with ghost cells." << std::endl;
    board.ghost_display();

    save_path = params.output_path + "step1.txt";
    board.save(save_path);
    
    return 0;
}
