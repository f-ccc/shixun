/* ============================================================
 * statistics.c - Statistical Analysis Implementation
 *
 * Each stat_* function extracts all records, groups/counts
 * by the relevant field, and prints a formatted table.
 * ============================================================ */

#include "statistics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stat_course_enrollment(DataManager *dm, int capacity) {
    int i, j, total, n, stat_count = 0;
    StudentRecord *all;
    typedef struct { char course_name[50], course_id[9]; int count; } CourseStat;
    CourseStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (CourseStat*)malloc(n * sizeof(CourseStat));
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++)
            if (!strcmp(stats[j].course_name, all[i].course_name)) { stats[j].count++; found = 1; break; }
        if (!found) {
            strcpy(stats[stat_count].course_name, all[i].course_name);
            strcpy(stats[stat_count].course_id, all[i].course_id);
            stats[stat_count].count = 1; stat_count++;
        }
    }
    printf("\n  === 每门课程选课人数统计 (预设容量: %d) ===\n", capacity);
    printf("  +------------------------------+----------+-------+---------+\n");
    printf("  | 课程名称                     | 课程编号 | 人数  | 使用率  |\n");
    printf("  +------------------------------+----------+-------+---------+\n");
    for (i = 0; i < stat_count; i++)
        printf("  | %-28s | %-8s | %5d | %5.1f%% |\n",
               stats[i].course_name, stats[i].course_id, stats[i].count,
               (float)stats[i].count / capacity * 100);
    printf("  +------------------------------+----------+-------+---------+\n");
    free(stats); free(all);
}

void stat_student_courses(DataManager *dm) {
    int i, j, total, n, stat_count = 0;
    StudentRecord *all;
    typedef struct { char student_id[13], name[20]; int course_count; float total_credit; } StudentStat;
    StudentStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (StudentStat*)malloc(n * sizeof(StudentStat));
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++)
            if (!strcmp(stats[j].student_id, all[i].student_id)) { stats[j].course_count++; stats[j].total_credit += all[i].credit; found = 1; break; }
        if (!found) {
            strcpy(stats[stat_count].student_id, all[i].student_id);
            strcpy(stats[stat_count].name, all[i].name);
            stats[stat_count].course_count = 1; stats[stat_count].total_credit = all[i].credit; stat_count++;
        }
    }
    printf("\n  === 学生选课门数与学分统计 ===\n");
    printf("  +--------------+----------+-------+-------+\n");
    printf("  | 学号         | 姓名     | 门数  | 总学分|\n");
    printf("  +--------------+----------+-------+-------+\n");
    for (i = 0; i < stat_count && i < 20; i++)
        printf("  | %-12s | %-8s | %5d | %6.1f |\n", stats[i].student_id, stats[i].name, stats[i].course_count, stats[i].total_credit);
    if (stat_count > 20) printf("  ... (共 %d 位学生)\n", stat_count);
    printf("  +--------------+----------+-------+-------+\n");
    free(stats); free(all);
}

void stat_college_distribution(DataManager *dm) {
    int i, j, total, n, stat_count = 0;
    StudentRecord *all;
    typedef struct { char college[30]; int count; } CollegeStat;
    CollegeStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (CollegeStat*)malloc(n * sizeof(CollegeStat));
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++)
            if (!strcmp(stats[j].college, all[i].college)) { stats[j].count++; found = 1; break; }
        if (!found) { strcpy(stats[stat_count].college, all[i].college); stats[stat_count].count = 1; stat_count++; }
    }
    printf("\n  === 各学院选课人数分布统计 ===\n");
    printf("  +----------------------------+-------+\n");
    printf("  | 学院                       | 人数  |\n");
    printf("  +----------------------------+-------+\n");
    for (i = 0; i < stat_count; i++) printf("  | %-27s | %5d |\n", stats[i].college, stats[i].count);
    printf("  +----------------------------+-------+\n");
    free(stats); free(all);
}

void stat_semester_distribution(DataManager *dm) {
    int i, j, k, total, n, stat_count = 0;
    StudentRecord *all;
    typedef struct { char semester[7]; int student_count, course_count, course_n; char courses[50][50]; } SemesterStat;
    SemesterStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (SemesterStat*)malloc(n * sizeof(SemesterStat));
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++)
            if (!strcmp(stats[j].semester, all[i].semester)) {
                stats[j].student_count++;
                int has_course = 0;
                for (k = 0; k < stats[j].course_n; k++) if (!strcmp(stats[j].courses[k], all[i].course_name)) { has_course = 1; break; }
                if (!has_course && stats[j].course_n < 50) { strcpy(stats[j].courses[stats[j].course_n], all[i].course_name); stats[j].course_n++; }
                found = 1; break;
            }
        if (!found) { strcpy(stats[stat_count].semester, all[i].semester); stats[stat_count].student_count = 1; strcpy(stats[stat_count].courses[0], all[i].course_name); stats[stat_count].course_n = 1; stat_count++; }
    }
    printf("\n  === 按学期统计选课人数与课程数分布 ===\n");
    printf("  +---------+-------+-------+\n");
    printf("  | 学期    | 人数  | 课程数|\n");
    printf("  +---------+-------+-------+\n");
    for (i = 0; i < stat_count; i++) printf("  | %-7s | %5d | %5d |\n", stats[i].semester, stats[i].student_count, stats[i].course_n);
    printf("  +---------+-------+-------+\n");
    free(stats); free(all);
}

void stat_score_distribution(DataManager *dm) {
    int i, total, n, s, excellent = 0, good = 0, medium = 0, pass = 0, fail = 0;
    StudentRecord *all;
    total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    for (i = 0; i < n; i++) {
        s = all[i].score;
        if (s >= 90) excellent++; else if (s >= 80) good++; else if (s >= 70) medium++; else if (s >= 60) pass++; else fail++;
    }
    printf("\n  === 课程成绩分数段分布统计 ===\n");
    printf("  +------+-------+-------+\n");
    printf("  | 等级 | 范围  | 人数  |\n");
    printf("  +------+-------+-------+\n");
    printf("  | A    | 90-100| %5d |\n", excellent);
    printf("  | B    | 80-89 | %5d |\n", good);
    printf("  | C    | 70-79 | %5d |\n", medium);
    printf("  | D    | 60-69 | %5d |\n", pass);
    printf("  | F    | <60   | %5d |\n", fail);
    printf("  +------+-------+-------+\n");
    printf("  | 总计 |       | %5d |\n", n);
    printf("  +------+-------+-------+\n");
    free(all);
}
