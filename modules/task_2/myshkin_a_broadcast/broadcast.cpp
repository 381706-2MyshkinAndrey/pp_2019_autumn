// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <random>
#include <stdexcept>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/myshkin_a_broadcast/broadcast.h"


int* getRandomArrayInt(int* buffer, int size) {
  if (size <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
      buffer[i] = gen() % 100;
  }
  return buffer;
}


double* getRandomArrayDouble(double* buffer, int size) {
  if (size <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
      buffer[i] = gen() % 100;
  }
  return buffer;
}


float* getRandomArrayFloat(float* buffer, int size) {
  if (size <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
      buffer[i] = gen() % 100;
  }
  return buffer;
}


int getMaxArrayInt(int* buffer, int size) {
  int max = buffer[0];
  for (int i = 0; i < size; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


double getMaxArrayDouble(double* buffer, int size) {
  double max = buffer[0];
  for (int i = 0; i < size; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


float getMaxArrayFloat(float* buffer, int size) {
  float max = buffer[0];
  for (int i = 0; i < size; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


void myBroadcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (count <= 0) {
    throw std::runtime_error("Error Array size");
  }

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != rank) {
        MPI_Send(buf, count, type, i, 1, comm);
      }
    }
  } else {
    MPI_Status Status;
    MPI_Recv(buf, count, type, root, 1, comm, &Status);
  }
}


double logFromTwo(int number) {
    double result = log(number) / log(2);
    return result;
}


void myQuickBroadcast(void* data, int count, MPI_Datatype datatype, int root,
    MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int tag = 1;
    int stepCount = ceil(logFromTwo(size));

    for (int step = 0; step < stepCount; step++) {
        int value = pow(2, step);
        if ((rank < value) && (value + rank < size)) {
            MPI_Send(data, count, datatype, (value + rank), tag, comm);
        }
        else if (rank < (2 * value) && rank >= value) {
            MPI_Status Status;
            MPI_Recv(data, count, datatype, (rank - value), tag, comm, &Status);
        }
    }
}
