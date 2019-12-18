// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
#define MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

int* getRandomArray(int sizeA);
int getMinArray(int* buffer, int sizeA);
int* ShellSortSenq(int* buffer, int length);

std::vector<int> getRandomVector(int length);
std::vector<int> ShellSortSenq(std::vector<int> buffer);

int* parallelShellSort(int* buffer, int length);
  // std::vector<int> shell_sort(std::vector<int> array);


#endif  // MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
