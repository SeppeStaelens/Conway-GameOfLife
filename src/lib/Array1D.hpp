#include <iostream>

#ifndef ARRAY1D_HPP
#define ARRAY1D_HPP

//! A class for 1D arrays
class Array1D{
    public:
        //! Size of the array
        int size;
        //! Pointer to the data
        int* data;

        //! Constructor
        Array1D(int size) {
            this -> size = size;
            this -> data = new int[size];
        }
        //! Destructor
        ~Array1D(){
            delete[] this -> data;
        }
        //! Overload the () operator to access the data
        int& operator()(int i){
            return this -> data[i];
        }

        //! Overwrite the data of the array with the data of another array
        //! @param arr The array to be copied into the current array
        //! @param shift The shift with which the array to be copied is loaded in the current array. If non-zero, arr needs to be smaller than the current array.
        void overwrite(Array1D arr, int shift = 0){
            for (int i = 0; i < arr.size; ++i){
                data[i + shift] = arr(i);
            }
        }

        //! Copy the data of the array into another array.
        //! @param arr The array from which the data is to be copied. Accessed by reference.
        void copy_into(Array1D* arr){
            for (int i = 0; i < size; ++i){
                data[i] = (*arr)(i);
            }
        }

        //! Create a subarray of the current array.
        //! @param i_low The lower index of the subarray
        //! @param i_upp The upper index of the subarray
        Array1D sub_arr(int i_low, int i_upp){
            int len;
            if (i_low > i_upp){
                len = size + i_upp - i_low;
            } else {
                len = i_upp - i_low;
            }
            Array1D sub(len);
            for (int i = 0; i < len; ++i) {
                sub(i) = data[(i_low + i) % size];
            }
            return sub;
        }

        //! Display the data of the array
        void display() {
            for (int i = 0; i < size; ++i) {
                    std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
};

#endif
