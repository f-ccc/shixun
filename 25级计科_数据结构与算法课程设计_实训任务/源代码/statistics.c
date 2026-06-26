/*
 * statistics.c - 统计分析实现
 */
#include "statistics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stat_course_enrollment(DataManager *dm, int capacity) {
    int i, j, total, n, stat_count;
    StudentRecord *all;
    typedef struct {
        char course_name[50];
        char course_id[9];
        int count;
    } CourseStat;
    CourseStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  \u6682\u65e0\u6570\u636e\u3002\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (CourseStat*)malloc(n * sizeof(CourseStat));
    stat_count = 0;
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
    printf("\n  === \u6bcf\u95e8\u8bfe\u7a0b\u9009\u8bfe\u4eba\u6570\u7edf\u8ba1 (\u5bb9\u91cf: %d) ===\n", capacity);
    printf("  +------------------------------+----------+-------+---------+\n");
    printf("  | \u8bfe\u7a0b\u540d\u79f0                     | \u8bfe\u7a0b\u7f16\u53f7 | \u4eba\u6570  | \u4f7f\u7528\u7387  |\n");
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
    int i, j, total, n, stat_count;
    StudentRecord *all;
    typedef struct {
        char student_id[13];
        char name[20];
        int course_count;
        float total_credit;
    } StudentStat;
    StudentStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  \u6682\u65e0\u6570\u636e\u3002\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (StudentStat*)malloc(n * sizeof(StudentStat));
    stat_count = 0;
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
    printf("\n  === \u6bcf\u4f4d\u5b66\u751f\u9009\u8bfe\u95e8\u6570\u4e0e\u603b\u5b66\u5206 ===\n");
    printf("  +--------------+----------+-------------+------------+\n");
    printf("  | \u5b66\u53f7         | \u59d3\u540d    | \u9009\u8bfe\u95e8\u6570   | \u603b\u5b66\u5206     |\n");
    printf("  +--------------+----------+-------------+------------+\n");
    for (i = 0; i < stat_count && i < 20; i++) {
        printf("  | %-12s | %-8s | %11d | %10.1f |\n",
               stats[i].student_id, stats[i].name,
               stats[i].course_count, stats[i].total_credit);
    }
    if (stat_count > 20) printf("  ... \u5171 %d \u4f4d\u5b66\u751f\n", stat_count);
    printf("  +--------------+----------+-------------+------------+\n");
    free(stats);
    free(all);
}

void stat_college_distribution(DataManager *dm) {
    int i, j, total, n, stat_count;
    StudentRecord *all;
    typedef struct {
        char college[30];
        int count;
    } CollegeStat;
    CollegeStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  \u6682\u65e0\u6570\u636e\u3002\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (CollegeStat*)malloc(n * sizeof(CollegeStat));
    stat_count = 0;
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
    printf("\n  === \u5404\u5b66\u9662\u9009\u8bfe\u4eba\u6570\u5206\u5e03 ===\n");
    printf("  +----------------------------+-------+\n");
    printf("  | \u5b66\u9662                       | \u4eba\u6570  |\n");
    printf("  +----------------------------+-------+\n");
    for (i = 0; i < stat_count; i++) {
        printf("  | %-27s | %5d |\n", stats[i].college, stats[i].count);
    }
    printf("  +----------------------------+-------+\n");
    free(stats);
    free(all);
}

void stat_semester_distribution(DataManager *dm) {
    int i, j, k, total, n, stat_count;
    StudentRecord *all;
    typedef struct {
        char semester[7];
        int student_count;
        int course_count;
        char courses[50][50];
        int course_n;
    } SemesterStat;
    SemesterStat *stats;
    total = dm_size(dm);
    if (total <= 0) { printf("  \u6682\u65e0\u6570\u636e\u3002\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    stats = (SemesterStat*)malloc(n * sizeof(SemesterStat));
    stat_count = 0;
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < stat_count; j++) {
            if (strcmp(stats[j].semester, all[i].semester) == 0) {
                stats[j].student_count++;
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
    printf("\n  === \u6309\u5b66\u671f\u7edf\u8ba1\u9009\u8bfe\u4eba\u6570\u4e0e\u8bfe\u7a0b\u6570\u5206\u5e03 ===\n");
    printf("  +---------+-------------+-------------+\n");
    printf("  | \u5b66\u671f    | \u9009\u8bfe\u4eba\u6570   | \u8bfe\u7a0b\u6570      |\n");
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
    int i, total, n, s;
    int excellent, good, medium, pass, fail;
    StudentRecord *all;
    total = dm_size(dm);
    if (total <= 0) { printf("  \u6682\u65e0\u6570\u636e\u3002\n"); return; }
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    excellent = 0; good = 0; medium = 0; pass = 0; fail = 0;
    for (i = 0; i < n; i++) {
        s = all[i].score;
        if (s >= 90) excellent++;
        else if (s >= 80) good++;
        else if (s >= 70) medium++;
        else if (s >= 60) pass++;
        else fail++;
    }
    printf("\n  === \u8bfe\u7a0b\u6210\u7ee9\u5206\u5e03\u7edf\u8ba1 ===\n");
    printf("  +----------+-------+---------+\n");
    printf("  | \u7b49\u7ea7     | \u5206\u6570\u6bb5 | \u4eba\u6570   |\n");
    printf("  +----------+-------+---------+\n");
    printf("  | \u4f18\u79c0     | 90-100 | %5d   |\n", excellent);
    printf("  | \u826f\u597d     | 80-89  | %5d   |\n", good);
    printf("  | \u4e2d\u7b49     | 70-79  | %5d   |\n", medium);
    printf("  | \u53ca\u683c     | 60-69  | %5d   |\n", pass);
    printf("  | \u4e0d\u53ca\u683c   | <60    | %5d   |\n", fail);
    printf("  +----------+-------+---------+\n");
    printf("  | \u5408\u8ba1     |       | %5d   |\n", n);
    printf("  +----------+-------+---------+\n");
    free(all);
}
