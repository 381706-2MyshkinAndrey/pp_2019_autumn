// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "./shell_sort.h"


TEST(Parallel_Shell_Sort, Test_With_Shell_Sort) {
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


TEST(Parallel_Shell_Sort, Test_With_Merge_Sort) {
    int rank;
    int sts = 0;
    int sts2 = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int length = 100000;
    int* tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
    int* tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));

    if (rank == 0) {
        sts = getRandomArray(tmp, length);
        if (tmp && tmp2) {
            memcpy(tmp2, tmp, (sizeof(int) * length));
        }

        double start2 = MPI_Wtime();
        sts2 = mergeSort(tmp2, length);
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


TEST(Parallel_Shell_Sort, Test_With_Shell_Sort_1) {
    int rank;
    int sts = 0;
    int sts2 = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int length = 129999;
    int* tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
    int* tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));

    if (rank == 0) {
        sts = getRandomArray(tmp, length);
        if (tmp && tmp2) {
            memcpy(tmp2, tmp, (sizeof(int) * length));
        }

        sts2 = ShellSortSenq(tmp2, length);
    }

    sts = parallelShellSort(tmp, length);

    if (rank == 0) {
        sts = sortingCheck(tmp, tmp2, length);
    }

    if (tmp) { free(tmp); tmp = nullptr; }
    if (tmp2) { free(tmp2); tmp2 = nullptr; }

    if (rank == 0) {
        ASSERT_EQ(sts, 0);
    }
}


TEST(Parallel_Shell_Sort, Test_With_Shell_Sort_2) {
    int rank;
    int sts = 0;
    int sts2 = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int length = 10000;
    int* tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
    int* tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));

    if (rank == 0) {
        sts = getRandomArray(tmp, length);
        if (tmp && tmp2) {
            memcpy(tmp2, tmp, (sizeof(int) * length));
        }

        sts2 = ShellSortSenq(tmp2, length);
    }

    sts = parallelShellSort(tmp, length);

    if (rank == 0) {
        sts = sortingCheck(tmp, tmp2, length);
    }

    if (tmp) { free(tmp); tmp = nullptr; }
    if (tmp2) { free(tmp2); tmp2 = nullptr; }

    if (rank == 0) {
        ASSERT_EQ(sts, 0);
    }
}


TEST(Parallel_Shell_Sort, Test_With_Shell_Sort_3) {
    int rank;
    int sts = 0;
    int sts2 = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int length = 35687;
    int* tmp = reinterpret_cast<int*>(malloc(length * sizeof(int)));
    int* tmp2 = reinterpret_cast<int*>(malloc(length * sizeof(int)));

    if (rank == 0) {
        sts = getRandomArray(tmp, length);
        if (tmp && tmp2) {
            memcpy(tmp2, tmp, (sizeof(int) * length));
        }

        sts2 = ShellSortSenq(tmp2, length);
    }

    sts = parallelShellSort(tmp, length);

    if (rank == 0) {
        sts = sortingCheck(tmp, tmp2, length);
    }

    if (tmp) { free(tmp); tmp = nullptr; }
    if (tmp2) { free(tmp2); tmp2 = nullptr; }

    if (rank == 0) {
        ASSERT_EQ(sts, 0);
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
