// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <vector>
#include <random>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/myshkin_a_shell_sort/shell_sort.h"

int getRandomArray(int* buffer, int size) {
  if ((size <= 0) || (buffer == nullptr)) return -1;
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
      buffer[i] = gen() % 100;
  }
  return 0;
}


int sortingCheck(int* buffer1, int* buffer2, int size) {
  if ((size <= 0) || (buffer1 == nullptr) || (buffer2 == nullptr)) return -1;
  for (int i = 1; i < size; i++) {
    if ((buffer1[i - 1] <= buffer1[i]) && (buffer2[i - 1] <= buffer2[i])) {
        i++;
    } else {
      return -1;
    }
  }
  
  for (int i = 0; i < size; i++) {
      if (buffer1[i] == buffer2[i]) {
          i++;
      } else {
          return -1;
      }
  }
  return 0;
}


int ShellSortSenq(int* buffer, int length) {
  if ((length <= 0) || (buffer == nullptr)) return -1;
  int i, j, temp;
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
  return 0;
}


void ownMergeBound(int* buffer, int left, int right, int middle)
{
    if (left >= right || middle < left || middle > right) return;
    if (right == left + 1 && buffer[left] > buffer[right]) {
        int value = buffer[right];
        buffer[right] = buffer[left];
        buffer[left] = value;
        return;
    }
    int* tmp = (int*)malloc(((right - left) + 1) * sizeof(int));
    if (tmp == nullptr) return;
    for (int i = 0; i < ((right - left) + 1); i++) tmp[i] = buffer[left + i];

    for (int i = left, j = 0, k = (middle - left + 1); i <= right; ++i) {
        if (j > middle - left) {
            buffer[i] = tmp[k++];
        }
        else if (k > right - left) {
            buffer[i] = tmp[j++];
        }
        else {
            buffer[i] = (tmp[j] < tmp[k]) ? tmp[j++] : tmp[k++];
        }
    }
    if (tmp) { free(tmp); tmp = nullptr; }

    return;
}

void ownMergeSort(int *buffer, int left, int right) {
  if (left >= right) return;

  int middle = left + (right - left) / 2;

  ownMergeSort(buffer, left, middle);
  ownMergeSort(buffer, (middle + 1), right);
  ownMergeBound(buffer, left, right, middle);
  return;
}

int mergeSort(int *buffer, int size) {
  if ((buffer == nullptr) || (size < 2)) return -1;
  int left = 0, right = size - 1;

  ownMergeSort(buffer, left, right);
  return 0;
}


int parallelShellSort(int* buffer, int length) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sizeBuf = length;

  if (sizeBuf <= 0) {
    return -1;
  }

  if (sizeBuf == 1) {
    return 0;
  }

  int sts = 0;
  int step = sizeBuf / 2;
  int sizeP, residue;
  int sizeProc;
  int counter = 0;
  
  while (step > 0) {
    residue = step % size;
    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    sizeProc = sizeBuf / step;
    int* bufForProc = (int*)(malloc(sizeProc * sizeof(int)));
    //int *bufForProc = reinterpret_cast<int*>(malloc(sizeProc * sizeof(int)));

    counter = 0;

    while (counter < step + sizeP - residue || counter <= step) {
      if (counter + residue == step) { sizeP = residue; }
      for (int proc = 0; proc < sizeP; proc++) {
        if (rank == 0) {
          for (int i = 0; i < sizeProc; i++) {
            bufForProc[i] = buffer[proc + counter + step * i];
          }
          if (proc == 0) {
            sts = mergeSort(bufForProc, sizeProc);
            if (sts == -1) {
                return -1;
            }
            for (int i = 0; i < sizeProc; i++) {
              buffer[counter + step * i] = bufForProc[i];
            }
          } else {
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, proc, 0, MPI_COMM_WORLD);
            /*printf("buf: ");
            for (int j = 0; j < sizeProc; j++) {
                if (j == (sizeProc - 1)) printf("%d\n", bufForProc[j]);
                else printf("%d, ", bufForProc[j]);
            }*/
          }
        } else {
          if (proc == rank) {
            MPI_Status Status;
            MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
            sts = mergeSort(bufForProc, sizeProc);
            if (sts == -1) {
                return -1;
            }
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, 0, 0, MPI_COMM_WORLD);
          }
        }
      }
      counter = counter + size;
    }

    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    counter = 0;
    
   while (counter < step + sizeP - residue || counter <= step) {
      if (counter + residue == step) { sizeP = residue; }
      for (int proc = 1; proc < sizeP; proc++) {
          if (rank == 0) {
              MPI_Status Status;
              MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, proc, 0, MPI_COMM_WORLD, &Status);
              for (int i = 0; i < sizeProc; i++) {
                  buffer[proc + step * i + counter] = bufForProc[i];
              }
          }
       }
        counter = counter + size;
    }
    step = step / 2;
    if (bufForProc) { free(bufForProc); bufForProc = nullptr; }
  }

  return 0;
}

