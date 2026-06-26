/*
 * filter_sort.h - 多条件筛选与多关键字排序模块
 */
#ifndef FILTER_SORT_H
#define FILTER_SORT_H
#include "record.h"
#include "operations.h"

int filter_records(DataManager *dm, FilterCondition *cond, StudentRecord *result, int max_result);
int multi_key_sort(StudentRecord *arr, int size, SortCondition *sort_keys, int key_count);
void print_record(const StudentRecord *rec);
void print_header(void);
void print_records(const StudentRecord *arr, int size);
void print_filter_condition(FilterCondition *cond);

#endif
