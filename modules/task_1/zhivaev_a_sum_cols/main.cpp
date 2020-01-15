// Copyright 2019 Zhivaev Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./sum_cols.h"

using std::vector;

TEST(Summ_Columns_MPI, Square_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 5;
    const int cols = rows;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Summ_Columns_Multi_Process_MPI, One_Row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 1, cols = 9;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Summ_Columns_Multi_Process_MPI, One_Column) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 9, cols = 1;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Summ_Columns_Multi_Process_MPI, Rectangle_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 9, cols = 5;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Summ_Columns_Multi_Process_MPI, Triangle_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 9;
    const int cols = rows;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = i + 1; j < cols; ++j) {
                global_mat[i * cols + j] = 0;
            }
        }
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

/*
TEST(Summ_Columns_Multi_Process_MPI, Large_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 400000;
    const int cols = 200;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
*/

TEST(Summ_Columns_Multi_Process_MPI, Empty_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 0;
    const int cols = 0;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    vector<int> global_sum = sumColumns(global_mat.data(), rows, cols);

    if (rank == 0) {
        vector<int> reference_sum = sumColumnsOneProc(global_mat.data(), rows, cols);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Summ_Columns_Multi_Process_MPI, Negative_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> global_mat;
    const int rows = 0;
    const int cols = 0;

    if (rank == 0) {
        global_mat = getRandomMatrix(rows, cols);
    }

    ASSERT_ANY_THROW(sumColumns(global_mat.data(), -1, -1));
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
