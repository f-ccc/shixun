/* ============================================================
 * benchmark.h - Performance Benchmark Interface
 *
 * run_benchmark: times insert/find/traverse/sort/delete
 * on all three data structures at a given data size.
 * Outputs comparison table + theoretical complexity.
 * ============================================================ */

#ifndef BENCHMARK_H
#define BENCHMARK_H
#include "record.h"
#include "operations.h"
#include <stddef.h>

void run_benchmark(int data_size);

#endif
