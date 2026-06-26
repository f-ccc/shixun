/*
 * filter_sort.h - 多条件筛选与多关键字排序模块
 */
#ifndef FILTER_SORT_H
#define FILTER_SORT_H
#include "record.h"
#include "operations.h"

/* 多条件筛选 */
int filter_records(DataManager *dm, FilterCondition *cond,
                   StudentRecord *result, int max_result);

/* 多关键字排序 */
int multi_key_sort(StudentRecord *arr, int size,
                   SortCondition *sort_keys, int key_count);

/* 打印筛选条件 */
void print_filter_condition(FilterCondition *cond);

/* 打印单条记录 */
void print_record(const StudentRecord *rec);

/* 打印记录列表 */
void print_records(const StudentRecord *arr, int size);

/* 显示记录表头 */
void print_header(void);

#endif
