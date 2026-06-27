/* ============================================================
 * filter_sort.h - Filter & Sort Interface
 *
 * filter_records: multi-condition query (course/semester/
 *   score range/college), supports fuzzy match.
 * multi_key_sort: bubble sort with multiple sort keys.
 * print_*: formatted table output helpers.
 * ============================================================ */

#ifndef FILTER_SORT_H
#define FILTER_SORT_H
#include "record.h"
#include "operations.h"

int filter_records(DataManager *dm, FilterCondition *cond, StudentRecord *result, int max_result);
int multi_key_sort(StudentRecord *arr, int size, SortCondition *sort_keys, int key_count);
void print_record(const StudentRecord *rec);
void print_header(void);
void print_records(const StudentRecord *arr, int size);

#endif
