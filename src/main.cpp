#include <iostream>
#include "lib/Board.hpp"
#include "lib/GameParams.hpp"
#include <random>

int main() {
    std::cout << std::endl;

    GameParams params;
    params.readParams("examples/params.txt"); // Create an object and initialize member variables from params file
    std::cout << "Parameters for the Game of Life." << std::endl;
    params.display(); // Display member variables
    std::cout << std::endl;
    
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

    Board sub(motherboard, 1, 4, 1, 3);
    sub.display();

    
    
    
    return 0;
}
