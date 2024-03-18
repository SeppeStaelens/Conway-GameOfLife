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
        int* getPointer() {
            return this -> data;
        }
        int getSize() {
            return this -> size;
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
        int* getPointer() {
            return this->data;
        }
        int getSize() {
            return this -> N_row * this -> N_col;
        }

        void display() {
            for (int i = 0; i < N_row; ++i) {
                for (int j = 0; j < N_col; ++j) {
                    std::cout << data[i*N_col+j] << " ";
                }
                std::cout << std::endl;
            }
        }

};

class Board : public Grid{

    public:

        Board(Grid motherboard, int row_low, int row_upp, int col_left, int col_right) : Grid(row_upp - row_low, col_right - col_left) {
            for (int i = 0; i < row_upp - row_low; ++i) {
                for (int j = 0; j < col_right - col_left; ++j) {
                    data[i*N_col+j] = motherboard(row_low + i, col_left + j);
                }
            }
        }

        ~Board(){
            delete[] this -> data;
        }

        // /*The ghost rows include the corners and are therefore wider than the board*/
        // Array1D<T>(N_col+2) bottom_ghost_row;
        // Array1D<T>(N_col+2) upper_ghost_row;
        // Array1D<T>(N_row) left_ghost_col;
        // Array1D<T>(N_row) right_ghost_col;

        // void set_bottom_ghost(Array1D<T> arr){
        //     bottom_ghost_row = arr;
        // }
        // void set_upper_ghost(Array1D<T> arr){
        //     upper_ghost_row = arr;
        // }
        // void set_left_ghost(Array1D<T> arr){
        //     left_ghost_col = arr;
        // }
        // void set_right_ghost(Array1D<T> arr){
        //     right_ghost_col = arr;
        // }

        Array1D get_bottom_row(){
            Array1D bottom(N_col);
            for (int i = 0; i < N_col; ++i) {
                bottom(i) = data[(N_row - 1) * N_col + i];
            }
            return bottom;          
        };

        // Array1D<T>(N_col) get_upper_row{
        //     return 1
        // }
        // Array1D<T>(N_col) get_left_col{
        //     return
        // }
        // Array1D<T>(N_col) get_right_col{
        //     return
        // }        

        // void count_neighbours(int i, int j){}

};

#endif
