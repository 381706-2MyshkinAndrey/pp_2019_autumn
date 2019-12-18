// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "./shell_sort.h"


TEST(Parallel_Shell_Sort, Test_First) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 10;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int min;

  if (rank == 0) {
    tmp = getRandomArray(length);
  }

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  if (rank == 0) {
    min = getMinArray(tmp, length);
  }
  free(tmp);

  if (rank == 0) {
    ASSERT_EQ(firstArray, min);
  }
}

TEST(Parallel_Shell_Sort, Test_Second) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 5;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int min;
  tmp[0] = 9;
  tmp[1] = 11;
  tmp[2] = 6;
  tmp[3] = 4;
  tmp[4] = 7;

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  if (rank == 0) {
    min = getMinArray(tmp, length);
  }
  free(tmp);

  if (rank == 0) {
    // int min = getMinArray(tmp, length);
    ASSERT_EQ(firstArray, min);
  }
}

TEST(Parallel_Shell_Sort, Test_Three) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 5;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  tmp[0] = 9;
  tmp[1] = 11;
  tmp[2] = 6;
  tmp[3] = 4;
  tmp[4] = 3;

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  free(tmp);

  if (rank == 0) {
    ASSERT_EQ(firstArray, 3);
  }
}

TEST(Parallel_Shell_Sort, Test_Four) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 100;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int min;

  if (rank == 0) {
    tmp = getRandomArray(length);
  }

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  if (rank == 0) {
    min = getMinArray(tmp, length);
  }
  free(tmp);

  if (rank == 0) {
    ASSERT_EQ(firstArray, min);
  }
}

TEST(Parallel_Shell_Sort, Test_Five) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 64;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int min;

  if (rank == 0) {
    tmp = getRandomArray(length);
  }

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  if (rank == 0) {
    min = getMinArray(tmp, length);
  }
  free(tmp);

  if (rank == 0) {
    ASSERT_EQ(firstArray, min);
  }
}

TEST(Parallel_Shell_Sort, Test_Six) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 80;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  // int *tmp1 = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int valSenq;

  if (rank == 0) {
    tmp = getRandomArray(length);
    int *tmp1 = reinterpret_cast<int*>(malloc(length * sizeof(int)));
    tmp1 = ShellSortSenq(tmp, length);
    valSenq = tmp1[0];
    free(tmp1);
  }

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  free(tmp);

  if (rank == 0) {
    ASSERT_EQ(firstArray, valSenq);
  }
}

TEST(Parallel_Shell_Sort, Test_Seven) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 0;
  int tmp[4];
  tmp[0] = 45;
  tmp[1] = 78;
  tmp[2] = 36;
  tmp[3] = 23;

  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomArray(length));
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
