/*
 * statistics.h - 统计分析模块接口
 */
#ifndef STATISTICS_H
#define STATISTICS_H
#include "record.h"
#include "operations.h"

void stat_course_enrollment(DataManager *dm, int capacity);
void stat_student_courses(DataManager *dm);
void stat_college_distribution(DataManager *dm);
void stat_semester_distribution(DataManager *dm);
void stat_score_distribution(DataManager *dm);

#endif
