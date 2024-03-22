#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <omp.h>

#include "Board.hpp"
#include "GameParams.hpp"

void initialize_random(Grid* grid, GameParams* params){
        
    // Use a random device to seed the random number engine
    std::random_device rd;
    std::mt19937 gen(rd());
    
    /* Define the Bernoulli distribution for generating random binary numbers (0 or 1).
        The parameter params.prob_live reflects the probability of generating a 1.*/
    std::bernoulli_distribution dist((*params).prob_live);

    // Fill the matrix with random binary numbers
    for (int i = 0; i < (*params).board_size; ++i) {
        for (int j = 0; j < (*params).board_size; ++j) {
            (*grid)(i,j) = dist(gen);
        }
    }

    std::cout << "Board initialized randomly." << std::endl << std::endl;
   
}


void initialize_from_file(Grid* grid, GameParams* params, std::string file) {

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
            (*grid)(i,j) = bit;
            j += 1;
        }
        j = 0;
        i += 1;

    }
    std::cout << "Board initialized from " << file << std::endl << std::endl;
}

void iteration_one_board(Board* board, GameParams* params, Array1D* store_row, Array1D* store_col){

    std::string path = (*params).output_path;

    std::cout << "Starting updating iteration." << std::endl;

    for (int i = 1; i <= (*params).evolve_steps; i++) {

        (*board).update_board();

        (*store_col).overwrite((*board).sub_col((*params).board_size - 1, 0, (*params).board_size));
        (*board).set_left_ghost_col(store_col);
        (*store_col).overwrite((*board).sub_col(0, 0, (*params).board_size));
        (*board).set_right_ghost_col(store_col);

        (*store_row).overwrite((*board).periodic_row((*params).board_size - 1));
        (*board).set_upper_ghost_row(store_row);
        (*store_row).overwrite((*board).periodic_row(0));
        (*board).set_bottom_ghost_row(store_row);

        if (i % (*params).save_interval == 0){
            path = (*params).output_path + "step" + std::to_string(i) +".txt";
            (*board).save(path);
        }
    }

}
