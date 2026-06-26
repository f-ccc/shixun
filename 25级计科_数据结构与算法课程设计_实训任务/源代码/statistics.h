/*
 * statistics.h - 统计分析模块接口
 */
#ifndef STATISTICS_H
#define STATISTICS_H
#include "record.h"
#include "operations.h"

/* 4.1 每门课程的选课人数与容量使用率 */
void stat_course_enrollment(DataManager *dm, int capacity);

/* 4.2 每位学生的选课门数与总学分 */
void stat_student_courses(DataManager *dm);

/* 4.3 各学院选课人数分布 */
void stat_college_distribution(DataManager *dm);

/* 4.4 按学期统计选课总人数与课程数分布 */
void stat_semester_distribution(DataManager *dm);

/* 4.5 课程成绩分布统计 */
void stat_score_distribution(DataManager *dm);

#endif
