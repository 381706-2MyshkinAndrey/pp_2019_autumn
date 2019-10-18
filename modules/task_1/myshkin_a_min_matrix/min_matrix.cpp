// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/myshkin_a_min_matrix/min_matrix.h"


std::vector<int> getConsistentMatrix(int m, int n) {
  if (m <= 0 || n <= 0)
    throw std::runtime_error("Error Matrix size");
  std::vector<int> matrix(m * n);
  for (int i = 0; i < n*m; i++) {
    matrix[i] = i;
  }
  return matrix;
}



std::vector<int> getRandomMatrix(int m, int n) {
  if (m <= 0 || n <= 0)
    throw std::runtime_error("Error Matrix size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> matrix(m * n);
  for (int i = 0; i < n*m; i++)
    matrix[i] = gen() % 100;
  return matrix;
}


int getSequentialMinMatrix(const std::vector<int> miv, int rows, int cols) {
  if (static_cast<unsigned int>(miv.size()) != rows * cols)
    throw std::runtime_error("Matrix dimensions are incorrect");
  int sizem = rows * cols;
  int min = miv[0];
  for (int i = 0; i < sizem; i++) {
    min = std::min(min, miv[i]);
  }
  return min;
}


int getParallelMinMatrix(const std::vector<int> miv, int rows, int cols) {
  int size, rank;
  int result = 0;
  int min;
  int error = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sizem = rows * cols;
  const int delta = sizem / size;
  const int remainder = sizem % size;

  std::vector<int> buf1(delta);

  if (sizem < size) {
    if (rank == 0) {
      result = getSequentialMinMatrix(miv, rows, cols);
      return result;
    } else {
      return result;
    }
  }


  if (rank == 0) {
    if (static_cast<unsigned int>(miv.size()) != rows * cols) {
      int error = -1;
      for (int proc = 1; proc < size; proc++)
        MPI_Send(&error, 1, MPI_INT, proc, -1, MPI_COMM_WORLD);
      throw std::runtime_error("Matrix dimensions are incorrect");
    }
  }


  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&miv[0] + proc * delta + remainder, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
    std::vector<int> miv_L = std::vector<int>(miv.begin(), miv.begin() + delta + remainder);
    min = getSequentialMinMatrix(miv_L, 1, delta + remainder);
  } else {
    MPI_Status Status;
    MPI_Recv(&buf1[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
    min = getSequentialMinMatrix(buf1, 1, delta);
  }

  MPI_Reduce(&min, &result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return result;
}
