/*
 * generator.h - 数据生成模块接口
 */
#ifndef GENERATOR_H
#define GENERATOR_H
#include "record.h"

/* 生成指定数量的测试数据 */
int generate_records(StudentRecord *records, int count);

/* 生成并保存到CSV文件 */
int generate_to_csv(const char *filename, int count);

#endif
