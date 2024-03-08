#ifndef BOARD_HPP
#define BOARD_HPP

template <typename T>
class Array1D{
    private:
        int size;
        T* data;

    public:
        Array1D(int size) {
            this -> size = size;
            this -> data = new T[size];
        }
        ~Array1D(){
            delete[] this -> data;
        }
        T& operator()(int i, int j){
            return this -> data[i];
        }
        T* getPointer() {
            return this -> data;
        }
        int getSize() {
            return this -> size;
        }
};

template <typename T>
class MotherBoard{
    private:
        int N_row;
        int N_col;
        T* data;

    public:
        MotherBoard(int N_row, int N_col){
            this -> N_row = N_row;
            this -> N_col = N_col;
            this -> data = new T[N_row*N_col];
        }
        ~MotherBoard(){
            delete[] this -> data;
        }
        T& operator()(int i, int j){
            return this -> data[i*N_col+j];
        }
        T* getPointer() {
            return this->data;
        }
        int getSize() {
            return this -> N_row * this -> N_col;
        }
};

template <typename T>
class Board : MotherBoard{

    public:
        /*The ghost rows include the corners and are therefore wider than the board*/
        Array1D<T>(N_col+2) bottom_ghost_row;
        Array1D<T>(N_col+2) upper_ghost_row;
        Array1D<T>(N_row) left_ghost_col;
        Array1D<T>(N_row) right_ghost_col;

        void set_bottom_ghost(Array1D<T> arr){
            bottom_ghost_row = arr;
        }
        void set_upper_ghost(Array1D<T> arr){
            upper_ghost_row = arr;
        }
        void set_left_ghost(Array1D<T> arr){
            left_ghost_col = arr;
        }
        void set_right_ghost(Array1D<T> arr){
            right_ghost_col = arr;
        }

        Array1D<T>(N_col) get_bottom_row{
            return data[(N_row - 1) * N_col : -1]           /*python style, likely change*/
        }
        Array1D<T>(N_col) get_upper_row{
            return 1
        }
        Array1D<T>(N_col) get_left_col{
            return
        }
        Array1D<T>(N_col) get_right_col{
            return
        }        

        void count_neighbours(int i, int j){}

        
    
};
