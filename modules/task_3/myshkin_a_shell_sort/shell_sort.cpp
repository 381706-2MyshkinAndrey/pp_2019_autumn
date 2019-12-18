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

  int sizeBuf = buffer.size();

  if (sizeBuf <= 0) {
    throw std::runtime_error("Error size of array");
  }

  if (sizeBuf == 1) {
    return buffer;
  }

  // int sizeArray = sizeBuf;

  // int sts = 0;
  int step = sizeBuf / 2;
  // int op = buffer.size();
  int sizeLocalArray, countProc, sizeP;
  // int count2;
  // int countOp;
  int tag = 0, counter = 0;

  while (step > 0) {  // op != 0
    // op = op / 2;
    // countOp = step / size;
    countProc = step % size;
    sizeP = step < size ? step : size;
    // count2 = countOp;
    // if (countProc > 0)
    //  count2++;
    tag = 0, counter = 0;
    sizeLocalArray = sizeBuf / step;
    std::vector<int> localArray(sizeLocalArray);
    std::vector<int> localArrayForRoot(sizeLocalArray);
    do {
      if (counter + countProc == step)
        sizeP = countProc;
      for (int proc = 0; proc < sizeP; proc++) {
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
            MPI_Send(&localArray[0], sizeLocalArray, MPI_INT, proc, tag, MPI_COMM_WORLD);
          }
        } else {
          if (rank == proc) {
            MPI_Status status;
            MPI_Recv(&localArray[0], sizeLocalArray, MPI_INT, 0,
              tag, MPI_COMM_WORLD, &status);
            // sts = mergeSort(localArray);
            localArray = ShellSortSenq(localArray);
            MPI_Send(&localArray[0], sizeLocalArray, MPI_INT, 0, proc + tag, MPI_COMM_WORLD);
          }
        }
      }
      counter = counter + size;
      // counter += size;
      tag++;
    } while (counter < step);

    sizeP = step < size ? step : size;
    counter = 0;
    tag = 0;

    if (rank == 0) {
      do {
        if (counter + countProc == step)
          sizeP = countProc;
        for (int proc = 1; proc < sizeP; proc++) {
          MPI_Status status;
          MPI_Recv(&localArray[0], sizeLocalArray, MPI_INT, proc,
            proc + tag, MPI_COMM_WORLD, &status);
          for (int i = 0; i < sizeLocalArray; i++) {
            buffer[proc + step * i + counter] = localArray[i];
          }
        }
        counter = counter + size;
        tag++;
      } while (counter < step);
    }

    // if (sizeLocalArray == sizeArray)
    //   step = 0;
    step = step / 2;
  }
  return buffer;
}
