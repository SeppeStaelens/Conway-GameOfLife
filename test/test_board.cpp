#include <gtest/gtest.h>
#include "../src/lib/Board.hpp"

TEST(Board, InitFromMotherboard){
    Board board(3, 3);
    Grid motherboard(5, 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            motherboard(i, j) = i * 5 + j;
        }
    }
    board.init_from_motherboard(&motherboard, 1, 1);
    EXPECT_EQ(board(0, 0), 6) << "Did not initialize correctly";
    EXPECT_EQ(board(0, 1), 7) << "Did not initialize correctly";
    EXPECT_EQ(board(0, 2), 8) << "Did not initialize correctly";
    EXPECT_EQ(board(1, 0), 11) << "Did not initialize correctly";
    EXPECT_EQ(board(1, 1), 12) << "Did not initialize correctly";
    EXPECT_EQ(board(1, 2), 13) << "Did not initialize correctly";
    EXPECT_EQ(board(2, 0), 16) << "Did not initialize correctly";
    EXPECT_EQ(board(2, 1), 17) << "Did not initialize correctly";
    EXPECT_EQ(board(2, 2), 18) << "Did not initialize correctly";
}

TEST(Board, storing_rows){
    Board board(3, 3);
    Grid motherboard(5, 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            motherboard(i, j) = i * 5 + j;
        }
    }
    board.init_from_motherboard(&motherboard, 1, 1);

    Array1D bottom_row_p(board.N_col + 2);
    Array1D top_row_p(board.N_col + 2);
    Array1D left_col(board.N_row);
    Array1D right_col(board.N_row);

    /* Store the edges of the board*/
    board.store_row(&bottom_row_p, board.N_row - 1, 1);
    board.store_row(&top_row_p, 0, 1);
    board.store_col(&left_col, 0);
    board.store_col(&right_col, board.N_col - 1);

    EXPECT_EQ(bottom_row_p(1), 16) << "Did not store bottom row correctly";
    EXPECT_EQ(bottom_row_p(2), 17) << "Did not store bottom row correctly";
    EXPECT_EQ(bottom_row_p(3), 18) << "Did not store bottom row correctly";

    EXPECT_EQ(top_row_p(1), 6) << "Did not store top row correctly";
    EXPECT_EQ(top_row_p(2), 7) << "Did not store top row correctly";
    EXPECT_EQ(top_row_p(3), 8) << "Did not store top row correctly";

    EXPECT_EQ(left_col(0), 6) << "Did not store left column correctly";
    EXPECT_EQ(left_col(1), 11) << "Did not store left column correctly";
    EXPECT_EQ(left_col(2), 16) << "Did not store left column correctly";

    EXPECT_EQ(right_col(0), 8) << "Did not store right column correctly";
    EXPECT_EQ(right_col(1), 13) << "Did not store right column correctly";
    EXPECT_EQ(right_col(2), 18) << "Did not store right column correctly";
}
