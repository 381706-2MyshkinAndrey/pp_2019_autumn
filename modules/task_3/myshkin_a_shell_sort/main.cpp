// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "./shell_sort.h"


TEST(Parallel_Shell_Sort, Test_With_Min) {
  int rank;
  int sts = 0;
  int sts2 = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 100000;
  int *tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
  int *tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));

  if (rank == 0) {
    sts = getRandomArray(tmp, length);
    if (tmp && tmp2) {
        memcpy(tmp2, tmp, (sizeof(int) * length));
    }
    // memcpy(tmp2, tmp, (sizeof(int) * length));
    /*if (*tmp) {
        for (int i = 0; i < length; i++) {
            tmp2[i] = tmp[i];
        }
    }*/
    /*for (int i = 0; i < length; i++) {
        tmp2[i] = tmp[i];
    }*/
    double start2 = MPI_Wtime();
    sts2 = ShellSortSenq(tmp2, length);
    double end2 = MPI_Wtime();
    printf(" Time the Senq Shell = %lf\n", end2 - start2);
  }

  double start = MPI_Wtime();
  sts = parallelShellSort(tmp, length);
  double end = MPI_Wtime();

  if (rank == 0) {
      printf(" Time the Parallel Shell = %lf\n", end - start);
      sts = sortingCheck(tmp, tmp2, length);
  }

  if (tmp) { free(tmp); tmp = nullptr; }
  if (tmp2) { free(tmp2); tmp2 = nullptr; }

  if (rank == 0) {
      ASSERT_EQ(sts, 0);
  }
}

//TEST(Parallel_Shell_Sort, Test_With_Merge) {
//    int rank;
//    int sts = 0;
//    int sts2 = 0;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//    const int length = 140000;
//    int* tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
//    int* tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));
//    // int* min;
//
//    if (rank == 0) {
//        sts = getRandomArray(tmp, length);
//        // memcpy(tmp2, tmp, (sizeof(int) * length));
//        /*if (*tmp) {
//            for (int i = 0; i < length; i++) {
//                tmp2[i] = tmp[i];
//            }
//        }*/
//        for (int i = 0; i < length; i++) {
//            tmp2[i] = tmp[i];
//        }
//        double start2 = MPI_Wtime();
//        sts2 = mergeSort(tmp2, length);
//        double end2 = MPI_Wtime();
//        printf(" Time the Senq Merge = %lf\n", end2 - start2);
//    }
//
//    double start = MPI_Wtime();
//    sts = parallelShellSort(tmp, length);
//    double end = MPI_Wtime();
//
//    if (rank == 0) {
//        printf(" Time the Parallel Shell = %lf\n", end - start);
//        sts = sortingCheck(tmp, tmp2, length);
//        //sts = sortingCheck(tmp, length);
//        //sts2 = sortingCheck(tmp2, length);
//    }
//    if (tmp) { free(tmp); tmp = nullptr; }
//    if (tmp2) { free(tmp2); tmp2 = nullptr; }
//    // free(tmp);
//    // free(tmp2);
//
//    if (rank == 0) {
//        ASSERT_EQ(sts, 0);
//    }
//}


  /*
TEST(Parallel_Shell_Sort, Test_With_Consistent_Array) {
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
    ASSERT_EQ(firstArray, min);
  }
}

TEST(Parallel_Shell_Sort, Test_With_Const) {
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

TEST(Parallel_Shell_Sort, Test_With_Large_Array) {
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

TEST(Parallel_Shell_Sort, Test_With_Normal_Array) {
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

TEST(Parallel_Shell_Sort, Test_With_Any_Throw) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length = 0;

  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomArray(length));
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
