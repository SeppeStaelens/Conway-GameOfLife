#ifndef GAMEPARAMS_HPP
#define GAMEPARAMS_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//! A class that stores the parameters for the Game of Life
class GameParams {
 public:
  //! The size of the board
  int board_size{10};
  //! The number of critical neighbours for a cell to survive
  int N_critical{3};
  //! The interval at which the board is saved
  int save_interval{1};
  //! The number of steps over which the board is evolved
  int evolve_steps{20};
  //! Whether to initialize the board with random data or from a file. 1:
  //! random, 0: file (board_file)
  int random_data{1};
  //! The number of OMP threads to use
  int num_threads{1};
  //! The probability that a cell is alive at the start, parameter in a Binomial
  //! distribution
  double prob_live{0.5};
  //! The path to the initialization file, in case random_data is 0
  std::string board_file{"examples/"};
  //! The path where to store the output files
  std::string output_path{"examples/"};

  //! Default constructor
  GameParams() {}

  //! Function that reads the parameters from a text file
  //! \param filename path to params file, parsed through command line
  void readParams(const std::string& filename) {
    std::ifstream inputFile(filename);  // Open the text file for reading

    if (!inputFile) {  // Check if the file was opened successfully
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
        } else if (paramName == "board_file") {
          std::istringstream(paramValue) >> board_file;
        } else if (paramName == "output_path") {
          std::istringstream(paramValue) >> output_path;
        } else if (paramName == "num_threads") {
          std::istringstream(paramValue) >> num_threads;
        }
      }
    }

    // Close the file
    inputFile.close();
  }

  //! Function that displays the parameters
  void display() const {
    std::cout << "board size: " << board_size << std::endl;
    std::cout << "N_critical: " << N_critical << std::endl;
    std::cout << "save interval: " << save_interval << std::endl;
    std::cout << "evolve steps: " << evolve_steps << std::endl;
    std::cout << "num omp threads: " << num_threads << std::endl;
    std::cout << "probability to live: " << prob_live << std::endl;
    if (random_data) {
      std::cout << "initialization: random" << std::endl;
    } else {
      std::cout << "initialization: " << board_file << std::endl;
    }
  }
};

#endif
