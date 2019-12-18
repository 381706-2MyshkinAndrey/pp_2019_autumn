// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <vector>
#include <random>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <utility>
#include "../../../modules/task_3/myshkin_a_shell_sort/shell_sort.h"


std::vector<int> getRandomVector(int length) {
  if (length <= 0)
    throw std::runtime_error("Error Vector size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vector(length);
  for (int i = 0; i < length; i++)
    vector[i] = gen() % 100;
  return vector;
}

std::vector<int> ShellSortSenq(std::vector<int> buffer) {
  int i, j, temp;
  int length = buffer.size();
  int step = length / 2;

  while (step > 0) {
    for (i = 0; i < (length - step); i++) {
      j = i;
      while (j >= 0 && buffer[j] > buffer[j + step]) {
        temp = buffer[j];
        buffer[j] = buffer[j + step];
        buffer[j + step] = temp;
        j--;
      }
    }
    step = step / 2;
  }
  return buffer;
}

std::vector<int> shell_sort(std::vector<int> buffer) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (buffer.size() <= 0) {
    throw std::runtime_error("Error size of array");
  }

  if (buffer.size() == 1) {
    return buffer;
  }

  int sizeArray = buffer.size();

  // int sts = 0;
  int step = buffer.size() / 2;
  // int op = buffer.size();
  int sizeLocalArray, countOp, countProc, count, count2;
  int n = 0, counter = 0;

  while (step > 0) {  // op != 0
    // op = op / 2;
    countOp = step / size;
    countProc = step % size;
    count = step < size ? step : size;
    count2 = countOp;
    if (countProc > 0)
      count2++;
    n = 0, counter = 0;
    sizeLocalArray = sizeArray / step;
    std::vector<int> localArray(sizeLocalArray);
    std::vector<int> localArrayForRoot(sizeLocalArray);
    do {
      if (counter + countProc == step)
        count = countProc;
      for (int proc = 0; proc < count; proc++) {
        if (rank == 0) {
          localArray.clear();
          for (int i = 0; i < sizeLocalArray; i++) {
            localArray.push_back(buffer[proc + counter + step * i]);
          }
          if (proc == 0) {
            // sts = mergeSort(localArray);
            localArray = ShellSortSenq(localArray);
            for (int i = 0; i < sizeLocalArray; i++) {
              buffer[counter + step * i] = localArray[i];
            }
          } else {
            MPI_Send(&localArray[0], sizeLocalArray, MPI_INT, proc, n, MPI_COMM_WORLD);
          }
        } else {
          if (rank == proc) {
            MPI_Status status;
            MPI_Recv(&localArray[0], sizeLocalArray, MPI_INT, 0,
              n, MPI_COMM_WORLD, &status);
            // sts = mergeSort(localArray);
            localArray = ShellSortSenq(localArray);
            MPI_Send(&localArray[0], sizeLocalArray, MPI_INT, 0, proc + n, MPI_COMM_WORLD);
          }
        }
      }
      counter = counter + size;
      // counter += size;
      n++;
    } while (counter < step);

    count = step < size ? step : size;
    counter = 0;
    n = 0;

    if (rank == 0) {
      do {
        if (counter + countProc == step)
          count = countProc;
        for (int proc = 1; proc < count; proc++) {
          MPI_Status status;
          MPI_Recv(&localArray[0], sizeLocalArray, MPI_INT, proc,
            proc + n, MPI_COMM_WORLD, &status);
          for (int i = 0; i < sizeLocalArray; i++) {
            buffer[proc + step * i + counter] = localArray[i];
          }
        }
        counter = counter + size;
        n++;
      } while (counter < step);
    }

    // if (sizeLocalArray == sizeArray)
    //   step = 0;
    step = step / 2;
  }
  return buffer;
}
