// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./min_matrix.h"


TEST(Parallel_Min_Matrix_MPI, Test_On_Min_Matrix_) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> global_matrix;
	const int rows_matrix_size = 50;
	const int cols_matrix_size = 50;

	if (rank == 0) {
		global_matrix = getRandomMatrix(rows_matrix_size, cols_matrix_size);
	}

	int global_min;
	global_min = getParallelMinMatrix(global_matrix, rows_matrix_size, cols_matrix_size);

	if (rank == 0) {
		int reference_min = getSequentialMinMatrix(global_matrix, rows_matrix_size, cols_matrix_size);
		ASSERT_EQ(global_min, reference_min);
	}
}

TEST(Parallel_Min_Matrix_MPI, Test_With_Consistent_Matrix) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> matrix;
	const int rows_matrix_size = 100;
	const int cols_matrix_size = 100;
	if (rank == 0) {
		matrix = getConsistentMatrix(rows_matrix_size, cols_matrix_size);
	}

	int min_matrix = getParallelMinMatrix(matrix, rows_matrix_size, cols_matrix_size);
	if (rank == 0) {
		ASSERT_EQ(0, min_matrix);
	}
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