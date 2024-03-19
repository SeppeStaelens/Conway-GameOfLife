#ifndef GAMEPARAMS_HPP
#define GAMEPARAMS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class GameParams{
    public:
        
        int board_size {10};
        int N_critical {3};
        int save_interval {1};
        int evolve_steps {20};
        int random_data {1};
        double prob_live {0.5};
        std::string output_path {"examples/"};

        GameParams(){} 

        void readParams(const std::string& filename){
        
            std::ifstream inputFile(filename); // Open the text file for reading

            if (!inputFile) { // Check if the file was opened successfully
                std::cerr << "Unable to open file " << filename << std::endl;
                return;
            }

            // Read parameters from the file and set member variables
            std::string line;
            while (std::getline(inputFile, line)) {

                if (line.empty() || line[0] == '#' || line.substr(0, 2) == "//") {
                    continue;
                }
 
                std::istringstream iss(line);
                std::string paramName, equalsSign, paramValue;

                // Parse the line into parameter name, '=', and parameter value
                if (iss >> paramName >> equalsSign >> paramValue && equalsSign == "=") {
                    // Set member variables based on parameter name
                    if (paramName == "board_size") {
                        std::istringstream(paramValue) >> board_size;
                    } else if (paramName == "N_critical") {
                        std::istringstream(paramValue) >> N_critical;
                    } else if (paramName == "save_interval") {
                        std::istringstream(paramValue) >> save_interval;
                    } else if (paramName == "num_evolve_steps") {
                        std::istringstream(paramValue) >> evolve_steps;
                    } else if (paramName == "random_data") {
                        std::istringstream(paramValue) >> random_data;
                    } else if (paramName == "prob_live") {
                        std::istringstream(paramValue) >> prob_live;
                    } else if (paramName == "output_path") {
                        std::istringstream(paramValue) >> output_path;
                    }
                }
            }

            // Close the file
            inputFile.close();
        }

        void display() const {
            std::cout << "board size: " << board_size << std::endl;
            std::cout << "N_critical: " << N_critical << std::endl;
            std::cout << "save interval: " << save_interval << std::endl;
            std::cout << "evolve steps: " << evolve_steps << std::endl;
            std::cout << "random data: " << random_data << std::endl;
            std::cout << "probability to live: " << prob_live << std::endl;
        }

};

#endif
