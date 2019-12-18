// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
#define MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int length);
std::vector<int> ShellSortSenq(std::vector<int> buffer);

std::vector<int> shell_sort(std::vector<int> array);
  // std::vector<int> getParallelShellSort(std::vector<int> buffer);


#endif  // MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
