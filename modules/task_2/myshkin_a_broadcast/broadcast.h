// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_

#include <mpi.h>

int* getRandomArrayInt(int* buffer, int size);  // for int
double* getRandomArrayDouble(double* buffer, int size);  // for double
float* getRandomArrayFloat(float* buffer, int size);  // for float

int getMaxArrayInt(int* buffer, int size);
double getMaxArrayDouble(double* buffer, int size);
float getMaxArrayFloat(float* buffer, int size);

void myBroadcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm);  // broadcast functions
void myQuickBroadcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_
