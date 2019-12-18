// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
#define MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

int* getRandomArrayInt(int sizeA);
int* ShellSortSenq(int* buffer, int length);

std::vector<int> getRandomVector(int length);
std::vector<int> ShellSortSenq(std::vector<int> buffer);

int* shell_sort(int* buffer, int length);
  // std::vector<int> shell_sort(std::vector<int> array);


#endif  // MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
