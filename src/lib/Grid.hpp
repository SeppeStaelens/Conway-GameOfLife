#include <omp.h>

#include <fstream>
#include <iostream>

#include "Array1D.hpp"

#ifndef GRID_HPP
#define GRID_HPP

//! A class for a 2D grid that contains the entire board for the Game of Life.
class Grid {
 public:
  //! Number of rows in the grid
  int N_row;
  //! Number of columns in the grid
  int N_col;
  //! Pointer to the data
  int* data;
  //! Number of critical neighbours used in the game rules
  int N_nb_crit;
  //! Number of rows times the number of columns
  int size;

  //! Constructor
  Grid(int N_row, int N_col, int N_nb_crit = 3) {
    this->N_row = N_row;
    this->N_col = N_col;
    this->N_nb_crit = N_nb_crit;
    this->data = new int[N_row * N_col];
    size = N_row * N_col;
  }
  //! Destructor
  ~Grid() { delete[] this->data; }

  //! Overload the () operator to access the data
  int& operator()(int i, int j) { return this->data[i * N_col + j]; }
  
  //! Store a row of the grid in an Array1D object
  //! @param store The Array1D object in which the row is to be stored
  //! @param n_row The index of the row to be stored
  //! @param shift The shift with which the row is loaded in the Array1D object

  void store_row(Array1D* store, int n_row, int shift = 0) {
#pragma omp parallel for
    for (int i = 0; i < N_col; ++i) {
      (*store)(i + shift) = data[n_row * N_col + i];
    }
  }

  //! Store a column of the grid in an Array1D object
  //! @param store The Array1D object in which the column is to be stored
  //! @param n_col The index of the column to be stored
  void store_col(Array1D* store, int n_col) {
#pragma omp parallel for
    for (int i = 0; i < N_row; ++i) {
      (*store)(i) = data[i * N_col + n_col];
    }
  }

  //! Return a subarray of a given row
  //! @param n_row The index of the row from which the subarray is to be taken
  //! @param i_low The lower index of the subarray
  //! @param i_upp The upper index of the subarray
  Array1D sub_row(int n_row, int i_low, int i_upp) {
    Array1D temp(N_col);
    store_row(&temp, n_row);
    return temp.sub_arr(i_low, i_upp);
  }

  //! Return a subarray of a given column
  //! @param n_col The index of the column from which the subarray is to be
  //! taken
  //! @param i_low The lower index of the subarray
  //! @param i_upp The upper index of the subarray
  Array1D sub_col(int n_col, int i_low, int i_upp) {
    Array1D temp(N_row);
    store_col(&temp, n_col);
    return temp.sub_arr(i_low, i_upp);
  }

  //! Display the data of the grid
  void display() {
    for (int i = 0; i < N_row; ++i) {
      for (int j = 0; j < N_col; ++j) {
        std::cout << data[i * N_col + j] << " ";
      }
      std::cout << std::endl;
    }
  }

  //! Return a row, with one cell added to the left and right, for periodic
  //! boundary conditions
  //! @param n_row The index of the row to be returned with the additional cells
  Array1D periodic_row(int n_row) {
    Array1D temp(N_col + 2);
    temp(0) = data[n_row * N_col + N_col - 1];
    store_row(&temp, n_row, 1);
    temp(N_col + 1) = data[n_row * N_col];
    return temp;
  }

  //! Save the data of the grid to a file
  //! @param file The name of the file to which the data is to be saved
  void save(std::string file) {
    std::ofstream outputFile(file);

    if (!outputFile.is_open()) {
      std::cerr << "Error opening file for writing!" << std::endl;
    }

    for (int i = 0; i < N_row; ++i) {
      for (int j = 0; j < N_col - 1; ++j) {
        outputFile << data[i * N_col + j] << " ";
      }
      outputFile << data[i * N_col + N_col - 1];
      outputFile << std::endl;
    }

    outputFile.close();
  }

  //! Store the data of the grid in an array
  //! @param arr The array in which the data is to be stored
  void store_data(int* arr) {
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
      arr[i] = data[i];
    }
  }

  //! Read the data of the grid from an array
  //! @param arr The array from which the data is to be read
  void read_data(int* arr) {
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
      data[i] = arr[i];
    }
  }

  //! Overwrite a subgrid of the grid with the data in an array
  //! @param arr The array from which the data is to be copied
  //! @param row_low The index of the lower row of the subgrid
  //! @param row_upp The index of the upper row of the subgrid
  //! @param col_low The index of the lower column of the subgrid
  //! @param col_upp The index of the upper column of the subgrid
  void overwrite_sub_board(int* arr, int row_low, int row_upp, int col_low,
                           int col_upp) {
    int n_rows = row_upp - row_low;
    int n_cols = col_upp - col_low;
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        data[(row_low + i) * N_col + col_low + j] = arr[i * n_cols + j];
      }
    }
  }
};

#endif
