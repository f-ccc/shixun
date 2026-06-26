/*
 * generator.h - 数据生成模块接口
 */
#ifndef GENERATOR_H
#define GENERATOR_H
#include "record.h"

int generate_records(StudentRecord *records, int count);
int generate_to_csv(const char *filename, int count);

#endif
