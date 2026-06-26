/*
 * fileio.h - CSV文件读写模块接口
 */
#ifndef FILEIO_H
#define FILEIO_H
#include "record.h"

int load_records(const char *filename, StudentRecord *records, int max_size);
int save_to_csv(const char *filename, StudentRecord *arr, int size);
int export_to_csv(const char *filename, StudentRecord *arr, int size);

#endif
