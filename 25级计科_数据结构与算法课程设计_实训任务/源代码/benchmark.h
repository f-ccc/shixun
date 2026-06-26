/*
 * benchmark.h - 性能测试模块接口
 */
#ifndef BENCHMARK_H
#define BENCHMARK_H
#include "record.h"
#include "operations.h"
#include <stddef.h>

/* 运行性能测试 */
void run_benchmark(int data_size);

/* 单操作计时（微秒级） */
double time_insert(void *ds, DataStructureType type, StudentRecord *data, int n);
double time_find(void *ds, DataStructureType type, StudentRecord *data, int n);
double time_delete(void *ds, DataStructureType type, StudentRecord *data, int n);
double time_traverse(void *ds, DataStructureType type, int n);
double time_sort(void *ds, DataStructureType type, int n);

/* 估算内存占用 */
size_t estimate_memory(void *ds, DataStructureType type);

#endif
