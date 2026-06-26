/*
 * fileio.h - CSV文件读写模块接口
 */
#ifndef FILEIO_H
#define FILEIO_H
#include "record.h"

/* 从CSV文件加载记录到数组 */
int load_from_csv(const char *filename, StudentRecord *arr, int max_size);

/* 从CSV文件加载记录到数组（同时返回实际数量） */
int load_records(const char *filename, StudentRecord *records, int max_size);

/* 保存记录数组到CSV文件 */
int save_to_csv(const char *filename, StudentRecord *arr, int size);

/* 导出筛选/排序结果到CSV文件 */
int export_to_csv(const char *filename, StudentRecord *arr, int size);

#endif
