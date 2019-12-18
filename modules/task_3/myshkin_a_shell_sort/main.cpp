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
  
  int sts = 0;
  const int length = 10;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));

  if (rank == 0) {
    tmp = getRandomArray(length);
  }

  tmp = parallelShellSort(tmp, length);
  int firstArray = tmp[0];
  free(tmp);

  if (rank == 0) {
    int min = getMinArray(tmp, length);
    ASSERT_EQ(firstArray, min);
  }
}

  /*
TEST(shell_sort, sort_decreasing_array) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> array = { 1, 2, 3, 4, 5, 6, 7 };

  std::vector<int> sortArray = shell_sort(array);

  if (rank == 0) {
    ASSERT_EQ(sortArray[0], 1);
  }
}

TEST(shell_sort, sort_decreasing_array2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 100;
  std::vector<int> array(length);

  if (rank == 0) {
    array = getRandomVector(length);
  }

  std::vector<int> sortArray = shell_sort(array);

  if (rank == 0) {
    // int senqVector = mergeSort(array);
    std::vector<int> senqVector = ShellSortSenq(array);
    ASSERT_EQ(sortArray, senqVector);
  }
}  */

  /*
TEST(Parallel_Shell_Sort, Test_On_Ordinary_Var1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  const int length = 8;
  std::vector<int> vectorTest(length);

  if (rank == 0) {
    vectorTest = getRandomVector(length);
  }

  vectorTest = getParallelShellSort(vectorTest);

  if (rank == 0) {
    std::vector<int> senqVector = mergeSort(vectorTest);
    ASSERT_EQ(vectorTest, senqVector);
  }
}


TEST(Parallel_Shell_Sort, Test_On_Ordinary_Var2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  const int length = 11;
  std::vector<int> vectorTest(length);

  if (rank == 0) {
    vectorTest = getRandomVector(length);
  }

  vectorTest = getParallelShellSort(vectorTest);

  if (rank == 0) {
    std::vector<int> senqVector = mergeSort(vectorTest);
    ASSERT_EQ(vectorTest, senqVector);
  }
}  */

  /********************************************
TEST(Parallel_Shell_Sort, Test_On_Ordinary_Var3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  const int length = 13;
  std::vector<int> vectorTest(length);

  if (rank == 0) {
    vectorTest = getRandomVector(length);
  }

  vectorTest = getParallelShellSort(vectorTest);

  if (rank == 0) {
    std::vector<int> senqVector = mergeSort(vectorTest);
    ASSERT_EQ(vectorTest[0], senqVector[0]);
  }
}  ***********************************************/

  /*
TEST(Parallel_Shell_Sort, Test_On_Large_Vector) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  const int length = 100;
  std::vector<int> vectorTest(length);

  if (rank == 0) {
    vectorTest = getRandomVector(length);
  }

  vectorTest = getParallelShellSort(vectorTest);

  if (rank == 0) {
    std::vector<int> senqVector = mergeSort(vectorTest);
    ASSERT_EQ(vectorTest, senqVector);
  }
}  */

  /*
TEST(Parallel_Shell_Sort, Test_With_Zero_Size) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  const int length = 0;
  std::vector<int> vectorTest(length);

  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomVector(length));
  }
}


TEST(Parallel_Shell_Sort, Test_With_Consistent_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vectorTest = { 6, 4, 3, 12, 10, 32, 76, 54 };

  vectorTest = getParallelShellSort(vectorTest);

  if (rank == 0) {
    ASSERT_EQ(3, vectorTest[0]);
  }
}  */


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
