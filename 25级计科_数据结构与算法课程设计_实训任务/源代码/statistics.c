/*
 * statistics.c - 统计分析实现
 */
#include "statistics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stat_course_enrollment(DataManager *dm, int capacity) {
    int i, j;
    int total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    /* 按课程分组统计 */
    typedef struct {
        char course_name[50];
        char course_id[9];
        int count;
    } CourseStat;

    CourseStat *stats = (CourseStat*)malloc(n * sizeof(CourseStat));
    int stat_count = 0;

    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++) {
            if (strcmp(stats[j].course_name, all[i].course_name) == 0) {
                stats[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(stats[stat_count].course_name, all[i].course_name);
            strcpy(stats[stat_count].course_id, all[i].course_id);
            stats[stat_count].count = 1;
            stat_count++;
        }
    }

    printf("\n  === 每门课程选课人数统计 (容量: %d) ===\n", capacity);
    printf("  +------------------------------+----------+-------+---------+\n");
    printf("  | 课程名称                     | 课程编号 | 人数  | 使用率  |\n");
    printf("  +------------------------------+----------+-------+---------+\n");
    for (i = 0; i < stat_count; i++) {
        float usage = (float)stats[i].count / capacity * 100;
        printf("  | %-28s | %-8s | %5d | %5.1f%% |\n",
               stats[i].course_name, stats[i].course_id,
               stats[i].count, usage);
    }
    printf("  +------------------------------+----------+-------+---------+\n");

    free(stats);
    free(all);
}

void stat_student_courses(DataManager *dm) {
    int i, j;
    int total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    typedef struct {
        char student_id[13];
        char name[20];
        int course_count;
        float total_credit;
    } StudentStat;

    StudentStat *stats = (StudentStat*)malloc(n * sizeof(StudentStat));
    int stat_count = 0;

    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++) {
            if (strcmp(stats[j].student_id, all[i].student_id) == 0) {
                stats[j].course_count++;
                stats[j].total_credit += all[i].credit;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(stats[stat_count].student_id, all[i].student_id);
            strcpy(stats[stat_count].name, all[i].name);
            stats[stat_count].course_count = 1;
            stats[stat_count].total_credit = all[i].credit;
            stat_count++;
        }
    }

    printf("\n  === 每位学生选课门数与总学分 ===\n");
    printf("  +--------------+----------+-------------+------------+\n");
    printf("  | 学号         | 姓名    | 选课门数   | 总学分     |\n");
    printf("  +--------------+----------+-------------+------------+\n");
    for (i = 0; i < stat_count && i < 20; i++) {
        printf("  | %-12s | %-8s | %11d | %10.1f |\n",
               stats[i].student_id, stats[i].name,
               stats[i].course_count, stats[i].total_credit);
    }
    if (stat_count > 20) printf("  ... 共 %d 位学生\n", stat_count);
    printf("  +--------------+----------+-------------+------------+\n");

    free(stats);
    free(all);
}

void stat_college_distribution(DataManager *dm) {
    int i, j;
    int total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    typedef struct {
        char college[30];
        int count;
    } CollegeStat;

    CollegeStat *stats = (CollegeStat*)malloc(n * sizeof(CollegeStat));
    int stat_count = 0;

    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++) {
            if (strcmp(stats[j].college, all[i].college) == 0) {
                stats[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(stats[stat_count].college, all[i].college);
            stats[stat_count].count = 1;
            stat_count++;
        }
    }

    printf("\n  === 各学院选课人数分布 ===\n");
    printf("  +----------------------------+-------+\n");
    printf("  | 学院                       | 人数  |\n");
    printf("  +----------------------------+-------+\n");
    for (i = 0; i < stat_count; i++) {
        printf("  | %-27s | %5d |\n", stats[i].college, stats[i].count);
    }
    printf("  +----------------------------+-------+\n");

    free(stats);
    free(all);
}

void stat_semester_distribution(DataManager *dm) {
    int i, j, k;
    int total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    typedef struct {
        char semester[7];
        int student_count;
        int course_count;
        /* 用数组记录课程 */
        char courses[50][50];
        int course_n;
    } SemesterStat;

    SemesterStat *stats = (SemesterStat*)malloc(n * sizeof(SemesterStat));
    int stat_count = 0;

    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++) {
            if (strcmp(stats[j].semester, all[i].semester) == 0) {
                stats[j].student_count++;
                /* 检查课程是否已记录 */
                int has_course = 0;
                for (k = 0; k < stats[j].course_n; k++) {
                    if (strcmp(stats[j].courses[k], all[i].course_name) == 0) {
                        has_course = 1;
                        break;
                    }
                }
                if (!has_course && stats[j].course_n < 50) {
                    strcpy(stats[j].courses[stats[j].course_n], all[i].course_name);
                    stats[j].course_n++;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(stats[stat_count].semester, all[i].semester);
            stats[stat_count].student_count = 1;
            strcpy(stats[stat_count].courses[0], all[i].course_name);
            stats[stat_count].course_n = 1;
            stat_count++;
        }
    }

    printf("\n  === 按学期统计选课人数与课程数分布 ===\n");
    printf("  +---------+-------------+-------------+\n");
    printf("  | 学期    | 选课人数   | 课程数      |\n");
    printf("  +---------+-------------+-------------+\n");
    for (i = 0; i < stat_count; i++) {
        printf("  | %-7s | %11d | %11d |\n",
               stats[i].semester, stats[i].student_count, stats[i].course_n);
    }
    printf("  +---------+-------------+-------------+\n");

    free(stats);
    free(all);
}

void stat_score_distribution(DataManager *dm) {
    int i;
    int total = dm_size(dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    int excellent = 0, good = 0, medium = 0, pass = 0, fail = 0;

    for (i = 0; i < n; i++) {
        int s = all[i].score;
        if (s >= 90) excellent++;
        else if (s >= 80) good++;
        else if (s >= 70) medium++;
        else if (s >= 60) pass++;
        else fail++;
    }

    printf("\n  === 课程成绩分布统计 ===\n");
    printf("  +----------+-------+---------+\n");
    printf("  | 等级     | 分数段 | 人数   |\n");
    printf("  +----------+-------+---------+\n");
    printf("  | 优秀     | 90-100 | %5d   |\n", excellent);
    printf("  | 良好     | 80-89  | %5d   |\n", good);
    printf("  | 中等     | 70-79  | %5d   |\n", medium);
    printf("  | 及格     | 60-69  | %5d   |\n", pass);
    printf("  | 不及格   | <60    | %5d   |\n", fail);
    printf("  +----------+-------+---------+\n");
    printf("  | 合计     |       | %5d   |\n", n);
    printf("  +----------+-------+---------+\n");

    free(all);
}
