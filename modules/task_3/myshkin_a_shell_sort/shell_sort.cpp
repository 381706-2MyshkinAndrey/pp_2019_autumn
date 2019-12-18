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

  // int sts = 0;
  int step = sizeBuf / 2;
  int sizeP, residue, sizeProc;
  int counter = 0;

  while (step > 0) {
    residue = step % size;
    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    sizeProc = sizeBuf / step;
    std::vector<int> bufForProc(sizeProc);

    counter = 0;

    while (counter <= step + sizeP - residue || counter <= step) {
      if (counter + residue == step) { sizeP = residue; }
      for (int proc = 0; proc < sizeP; proc++) {
        if (rank == 0) {
          bufForProc.clear();
          for (int i = 0; i < sizeProc; i++) {
            bufForProc.push_back(buffer[proc + counter + step * i]);
          }
          if (proc == 0) {
            // sts = mergeSort(localArray);
            bufForProc = ShellSortSenq(bufForProc);
            for (int i = 0; i < sizeProc; i++) {
              buffer[counter + step * i] = bufForProc[i];
            }
          } else {
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, proc, 0, MPI_COMM_WORLD);
          }
        } else {
          if (rank == proc) {
            MPI_Status Status;
            MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
            // sts = mergeSort(localArray);
            bufForProc = ShellSortSenq(bufForProc);
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, 0, proc, MPI_COMM_WORLD);
          }
        }
      }
      counter = counter + size;
    }  // while (counter < step);

    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    counter = 0;

    if (rank == 0) {
      while (counter <= step + sizeP - residue || counter <= step) {
        if (counter + residue == step) { sizeP = residue; }
        for (int proc = 1; proc < sizeP; proc++) {
          MPI_Status Status;
          MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, proc,
            proc, MPI_COMM_WORLD, &Status);
          for (int i = 0; i < sizeProc; i++) {
            buffer[proc + step * i + counter] = bufForProc[i];
          }
        }
        counter = counter + size;
      }  // while (counter < step);
    }
    step = step / 2;
  }
  return buffer;
}
