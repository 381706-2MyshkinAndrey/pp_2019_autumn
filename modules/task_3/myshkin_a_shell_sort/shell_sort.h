// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
#define MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

int getRandomArray(int* buffer, int size);
int sortingCheck(int* buffer1, int* buffer2, int size);

int ShellSortSenq(int* buffer, int length);
int mergeSort(int* buffer, unsigned int size);

int parallelShellSort(int* buffer, int length);


#endif  // MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
