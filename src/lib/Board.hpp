#include <iostream>
#include <fstream>

#ifndef BOARD_HPP
#define BOARD_HPP

class Array1D{
    private:
        int size;
        int* data;

    public:
        Array1D(int size) {
            this -> size = size;
            this -> data = new int[size];
        }
        ~Array1D(){
            delete[] this -> data;
        }
        int& operator()(int i){
            return this -> data[i];
        }

        void overwrite(Array1D arr){
            for (int i = 0; i < size; ++i){
                data[i] = arr(i);
            }
        }

        Array1D sub_arr(int i_low, int i_upp){
            int len = i_upp - i_low;
            Array1D sub(len);
            for (int i = 0; i < len; ++i) {
                sub(i) = data[i_low + i];
            }
            std::cout << "this" << std::endl;
            sub.display();
            return sub;
        }

        void display() {
            for (int i = 0; i < size; ++i) {
                    std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
};

class Grid{

    public:

        int N_row;
        int N_col;
        int* data;

        Grid(int N_row, int N_col){
            this -> N_row = N_row;
            this -> N_col = N_col;
            this -> data = new int[N_row*N_col];
        }
        ~Grid(){
            delete[] this -> data;
        }
        int& operator()(int i, int j){
            return this -> data[i*N_col+j];
        }

        void store_row(Array1D* store, int n_row, int shift = 0) {
            for (int i = 0; i < N_col; ++i) {
                (*store)(i + shift) = data[n_row * N_col + i];
            } 
        }
        void store_col(Array1D* store, int n_col) {
            for (int i = 0; i < N_row; ++i) {
                (*store)(i) = data[i * N_col + n_col];
            } 
        }

        Array1D sub_row(int n_row, int i_low, int i_upp){
            Array1D temp(N_col);
            store_row(&temp, n_row);
            return temp.sub_arr(i_low, i_upp);
        }

        Array1D sub_col(int n_col, int i_low, int i_upp){
            Array1D temp(N_row);
            store_col(&temp, n_col);
            return temp.sub_arr(i_low, i_upp);
        }

        void display() {
            for (int i = 0; i < N_row; ++i) {
                for (int j = 0; j < N_col; ++j) {
                    std::cout << data[i*N_col+j] << " ";
                }
                std::cout << std::endl;
            }
        }

        Array1D periodic_row(int n_row){
            Array1D temp(N_col + 2);
            temp(0) = data[n_row * N_col + N_col - 1];
            store_row(&temp, n_row, 1);
            temp(N_col + 1) = data[n_row * N_col];
            return temp;
        }

        void save(std::string file) {
            std::cout << file;
            std::ofstream outputFile(file);
            
            if (!outputFile.is_open()) {
                std::cerr << "Error opening file for writing!" << std::endl;
            }

            for (int i = 0; i < N_row; ++i) {
                for (int j = 0; j < N_col; ++j) {
                    outputFile << data[i*N_col+j] << " ";
                }
                outputFile << std::endl;
            }

            outputFile.close();
        }

};

class Board : public Grid{

    public:

        /*The ghost rows include the corners and are therefore wider than the board*/
        Array1D bottom_ghost_row;
        Array1D upper_ghost_row;
        Array1D left_ghost_col;
        Array1D right_ghost_col;

        Array1D temp1, temp2, temp3;

        // Constructor and initialization of size of the arrays
        Board(Grid* motherboard, int row_low, int row_upp, int col_left, int col_right) : Grid(row_upp - row_low, col_right - col_left), 
        bottom_ghost_row(N_col+2), upper_ghost_row(N_col+2), left_ghost_col(N_row), right_ghost_col(N_row),
         temp1(N_col), temp2(N_col), temp3(N_col) {
            for (int i = 0; i < N_row; ++i) {
                for (int j = 0; j < N_col; ++j) {
                    data[i*N_col+j] = (*motherboard)(row_low + i, col_left + j);
                }
            }
            /*do something that demands N_row to be at least 3*/
        }

        ~Board(){
            delete[] this -> data;
        }

        void set_bottom_ghost_row(Array1D* target) {
            for (int i = 0; i < N_col+2; ++i) {
                bottom_ghost_row(i) = (*target)(i);
            }
        }
        void set_upper_ghost_row(Array1D* target) {
            for (int i = 0; i < N_col+2; ++i) {
                upper_ghost_row(i) = (*target)(i);
            }
        }

        void set_left_ghost_col(Array1D* target) {
            for (int i = 0; i < N_col; ++i) {
                left_ghost_col(i) = (*target)(i);
            }
        }
        void set_right_ghost_col(Array1D* target) {
            for (int i = 0; i < N_col; ++i) {
                right_ghost_col(i) = (*target)(i);
            }
        }

        void store_neighbour_row(Array1D* store, int n_row) {
            (*store)(0) = left_ghost_col(n_row) + data[n_row * N_col + 0] + data[n_row * N_col + 1];
            for (int i = 1; i < N_col-1; ++i) {
                (*store)(i) = data[n_row * N_col + i-1] + data[n_row * N_col + i] 
                                + data[n_row * N_col + i + 1];
            } 
            (*store)(N_col - 1) = data[n_row * N_col + N_col - 2] + data[n_row * N_col + N_col - 1] + right_ghost_col(n_row);    
        }

        void store_upper_ghost_neighbour_row(Array1D* store) {
            for (int i = 0; i < N_col+1; ++i) {
                (*store)(i) = upper_ghost_row(i) + upper_ghost_row(i+1) + upper_ghost_row(i+2);
            }
        }
        void store_bottom_ghost_neighbour_row(Array1D* store) {
            for (int i = 0; i < N_col; ++i) {
                (*store)(i) = bottom_ghost_row(i) + bottom_ghost_row(i+1) + bottom_ghost_row(i+2);
            }
        }

        void ghost_display(){
            upper_ghost_row.display();

            for (int i = 0; i < N_row; ++i) {
                std::cout << left_ghost_col(i) << " ";
                for (int j = 0; j < N_col; ++j) {
                    std::cout << data[i*N_col+j] << " ";
                }
                std::cout << right_ghost_col(i) << std::endl;
            }

            bottom_ghost_row.display();
        }

        void update_board(){
            int N_nb {0};
            int val {0};
            store_upper_ghost_neighbour_row(&temp1);
            store_neighbour_row(&temp2, 0);
            store_neighbour_row(&temp3, 1);
            for (int j = 0; j < N_col; ++j) {
                val = data[j];
                N_nb = temp1(j) + temp2(j) + temp3(j) - val;
                data[j] = (1 - val) * (N_nb == 3) + val * (N_nb == 3 || N_nb == 2);
            }
            for (int i = 1; i < N_row -1; ++i){
                temp1.overwrite(temp2);
                temp2.overwrite(temp3);
                store_neighbour_row(&temp3, i+1);
                for (int j = 0; j < N_col; ++j) {
                    val = data[i*N_col + j];
                    N_nb = temp1(j) + temp2(j) + temp3(j) - val;
                    data[i*N_col + j] = (1 - val) * (N_nb == 3) + val * (N_nb == 3 || N_nb == 2);
                }
            }
            temp1.overwrite(temp2);
            temp2.overwrite(temp3);
            store_bottom_ghost_neighbour_row(&temp3);
            for (int j = 0; j < N_col; ++j) {
                val = data[(N_row - 1)*N_col + j];
                N_nb = temp1(j) + temp2(j) + temp3(j) - val;
                data[(N_row - 1)*N_col + j] = (1 - val) * (N_nb == 3) + val * (N_nb == 3 || N_nb == 2);
            }
            std::cout << &temp1 << std::endl << &temp2 << std::endl << &temp3;
        }

};

#endif
