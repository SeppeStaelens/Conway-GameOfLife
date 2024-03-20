#include <iostream>
#include "lib/Board.hpp"
#include "lib/GameParams.hpp"
#include <random>

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
        
        // Define the distribution for generating random binary numbers (0 or 1)
        std::bernoulli_distribution dist(params.prob_live); // Probability of success (getting 1) is 0.5

        // Fill the matrix with random binary numbers
        for (int i = 0; i < params.board_size; ++i) {
            for (int j = 0; j < params.board_size; ++j) {
                motherboard(i,j) = dist(gen);
            }
        }
    }

    motherboard.display();
    std::cout << std::endl; 

    Board sub(&motherboard, 2, 5, 2, 5);
    sub.display();

    Array1D test3 = motherboard.sub_row(1, 1, 6);
    sub.set_upper_ghost_row(&test3);

    test3.overwrite(motherboard.sub_row(5, 1, 6));
    sub.set_bottom_ghost_row(&test3);

    Array1D col = motherboard.sub_col(1, 2, 5);
    sub.set_left_ghost_col(&col);

    col.overwrite(motherboard.sub_col(5, 2, 5));
    sub.set_right_ghost_col(&col);

    std::cout << "dis" << std::endl;

    sub.ghost_display();
    
    std::cout << std::endl << "overwrite" << std::endl;
    sub.update_board();
    sub.display();
    std::string save_path = params.output_path + "test.txt";
    sub.save(save_path);
    
    return 0;
}
