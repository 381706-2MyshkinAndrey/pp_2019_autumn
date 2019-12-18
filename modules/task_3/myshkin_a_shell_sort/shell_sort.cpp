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

  /*
void ownMergeBound(std::vector<int> &buffer, size_t left, size_t right, size_t middle) {
  if (left >= right || middle < left || middle > right) return;
  if (right == left + 1 && buffer[left] > buffer[right]) {
    std::swap(buffer[left], buffer[right]);
    return;
  }

  std::vector<int> tmp(&buffer[left], &buffer[right] + 1);

  for (size_t i = left, j = 0, k = (middle - left + 1); i <= right; ++i) {
    if (j > middle - left) {
      buffer[i] = tmp[k++];
    } else if (k > right - left) {
      buffer[i] = tmp[j++];
    } else {
      buffer[i] = (tmp[j] < tmp[k]) ? tmp[j++] : tmp[k++];
    }
  }
  return;
}

void ownMergeSort(std::vector<int> &buffer, size_t left, size_t right) {
  if (left >= right) return;

  size_t middle = left + (right - left) / 2;

  ownMergeSort(buffer, left, middle);
  ownMergeSort(buffer, (middle + 1), right);
  ownMergeBound(buffer, left, right, middle);
  return;
}

int mergeSort(std::vector<int> &buffer) {
  size_t size = buffer.size();
  if (size == 0) return -1;
  size_t left = 0, right = size - 1;

  ownMergeSort(buffer, left, right);
  return 0;
}
  */

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

  /*
std::vector<int> getParallelShellSort(std::vector<int> buffer) {
  int size, rank;
  int sizeBuf = buffer.size();
  int step = sizeBuf / 2;
  int sts = 0;
  // int k = 1;
  int sizeP = 0;
  // int tag = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (buffer.size() <= 0) {
    throw std::runtime_error("Error size of array");
  }

  if (buffer.size() == 1) {
    return buffer;
  }

  while (step > 0) {
    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }
    int remainder = step % size;
    // int remainder = step / size;
    int sizeLocal = sizeBuf / step;
    if (remainder != 0) {
      sts = 1;
    }
    // std::vector<int> bufRem(sizeLocal + remainder);
    // std::vector<int> bufferForSend(sizeLocal);
    std::vector<int> bufferForRecv(sizeLocal);
    int counter = 0;
    // tag = 0;

    while (counter < step) {
      if (sts == 1 && counter + remainder == step && (remainder < sizeP)) { sizeP = remainder; }  // sizeP
      for (int proc = 0; proc < sizeP; proc++) {
        if (rank == 0) {
          // bufferForSend.clear();  // Send
          bufferForRecv.clear();
          for (int i = 0; i < sizeLocal; i++) {
            bufferForRecv.push_back(buffer[i * step + proc + counter]);  // Send
          }
          if (proc == 0) {
            bufferForRecv = mergeSort(bufferForRecv);
            for (int j = 0; j < sizeLocal; j++) {
              buffer[j * step + counter] = bufferForRecv[j];
            }
          } else {
            MPI_Send(&bufferForRecv[0], sizeLocal, MPI_INT, proc, 0, MPI_COMM_WORLD);  // ?
          }
        } else {
          if (proc == rank && proc != 0) {
            MPI_Status Status;
            MPI_Recv(&bufferForRecv[0], sizeLocal, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
            // k++;
            // bufferForSend = mergeSort(bufferForRecv);
            bufferForRecv = mergeSort(bufferForRecv);

            MPI_Send(&bufferForRecv[0], sizeLocal, MPI_INT, 0, proc, MPI_COMM_WORLD);
            // tag++;
            // k++;
          }
        }
      }
      // tag = 0;
      if (size > step) {
        sizeP = step;
      } else {
        sizeP = size;
      }
      if (sizeP > 1) {
        for (int proc = 1; proc < sizeP; proc++) {
          if (rank == 0) {
            MPI_Status Status;
            MPI_Recv(&bufferForRecv[0], sizeLocal, MPI_INT, proc, proc, MPI_COMM_WORLD, &Status);

            // bufferForSend = mergeSort(bufferForRecv);

            for (int j = 0; j < sizeLocal; j++) {
              buffer[j * step + proc + counter] = bufferForRecv[j];
            }
          }
        }
      }
      counter = counter + size;
    }
    counter = 0;
    step = step / 2;
  }
  return buffer;
}  */


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

  int sts = 0;
  int op = buffer.size();
  int sizeLocalArray, countOp, countProc, count, count2;
  int n = 0, counter = 0;

  while (op != 0) {
    op = op / 2;
    countOp = op / size;
    countProc = op % size;
    count = op < size ? op : size;
    count2 = countOp;
    if (countProc > 0)
      count2++;
    n = 0, counter = 0;
    sizeLocalArray = sizeArray / op;
    std::vector<int> localArray(sizeLocalArray);
    std::vector<int> localArrayForRoot(sizeLocalArray);
    do {
      if (counter + countProc == op)
        count = countProc;
      for (int proc = 0; proc < count; proc++) {
        if (rank == 0) {
          localArray.clear();
          for (int i = 0; i < sizeLocalArray; i++) {
            localArray.push_back(buffer[proc + counter + op * i]);
          }
          if (proc == 0) {
            // sts = mergeSort(localArray);
            localArray = ShellSortSenq(localArray);
            for (int i = 0; i < sizeLocalArray; i++) {
              buffer[counter + op * i] = localArray[i];
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
    } while (counter < op);

    count = op < size ? op : size;
    counter = 0;
    n = 0;

    if (rank == 0) {
      do {
        if (counter + countProc == op)
          count = countProc;
        for (int proc = 1; proc < count; proc++) {
          MPI_Status status;
          MPI_Recv(&localArray[0], sizeLocalArray, MPI_INT, proc,
            proc + n, MPI_COMM_WORLD, &status);
          for (int i = 0; i < sizeLocalArray; i++) {
            buffer[proc + op * i + counter] = localArray[i];
          }
        }
        counter = counter + size;
        n++;
      } while (counter < op);
    }
    if (sizeLocalArray == sizeArray)
      op = 0;
  }
  return buffer;
}
