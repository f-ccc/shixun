/*
 * filter_sort.c - 多条件筛选与多关键字排序实现
 */
#include "filter_sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int filter_records(DataManager *dm, FilterCondition *cond,
                   StudentRecord *result, int max_result) {
    int i;
    if (!dm) return 0;

    /* 先获取所有记录 */
    int total = dm_size(dm);
    if (total <= 0) return 0;

    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(dm, all, total);

    int count = 0;
    for (i = 0; i < n && count < max_result; i++) {
        int match = 1;

        /* 课程名称筛选 */
        if (cond->course_name[0] != '\0') {
            if (cond->fuzzy_match) {
                if (strstr(all[i].course_name, cond->course_name) == NULL)
                    match = 0;
            } else {
                if (strcmp(all[i].course_name, cond->course_name) != 0)
                    match = 0;
            }
        }

        /* 学期筛选 */
        if (match && cond->semester[0] != '\0') {
            if (strcmp(all[i].semester, cond->semester) != 0)
                match = 0;
        }

        /* 成绩区间筛选 */
        if (match && cond->score_min != -1) {
            if (all[i].score < cond->score_min) match = 0;
        }
        if (match && cond->score_max != -1) {
            if (all[i].score > cond->score_max) match = 0;
        }

        /* 学院筛选 */
        if (match && cond->college[0] != '\0') {
            if (strcmp(all[i].college, cond->college) != 0)
                match = 0;
        }

        if (match) {
            result[count++] = all[i];
        }
    }
    free(all);
    return count;
}

int multi_key_sort(StudentRecord *arr, int size,
                   SortCondition *sort_keys, int key_count) {
    int i, j, k;
    if (size <= 1 || key_count <= 0) return size;

    /* 使用冒泡排序实现多关键字排序 */
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - 1 - i; j++) {
            /* 多关键字比较 */
            int need_swap = 0;
            for (k = 0; k < key_count; k++) {
                int cmp = 0;
                const char *field = sort_keys[k].field;

                if (strcmp(field, "score") == 0 || strcmp(field, "成绩") == 0) {
                    cmp = arr[j].score - arr[j+1].score;
                } else if (strcmp(field, "student_id") == 0 || strcmp(field, "学号") == 0) {
                    cmp = strcmp(arr[j].student_id, arr[j+1].student_id);
                } else if (strcmp(field, "name") == 0 || strcmp(field, "姓名") == 0) {
                    cmp = strcmp(arr[j].name, arr[j+1].name);
                }

                if (cmp != 0) {
                    need_swap = sort_keys[k].dir ? (cmp > 0) : (cmp < 0);
                    break;
                }
            }

            if (need_swap) {
                StudentRecord tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
    return size;
}

void print_record(const StudentRecord *rec) {
    printf("| %-12s | %-8s | %-16s | %-8s | %-20s | %.1f | %-7s | %-10s | %3d |\n",
           rec->student_id, rec->name, rec->college,
           rec->course_id, rec->course_name,
           rec->credit, rec->semester,
           rec->enroll_date, rec->score);
}

void print_header(void) {
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    printf("| 学号         | 姓名    | 学院             | 课程编号 | 课程名称             | 学分 | 学期   | 选课日期   | 成绩 |\n");
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
}

void print_records(const StudentRecord *arr, int size) {
    int i;
    if (size <= 0) {
        printf("  (无记录)\n");
        return;
    }
    print_header();
    for (i = 0; i < size; i++) {
        print_record(&arr[i]);
    }
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    printf("  共 %d 条记录\n", size);
}

void print_filter_condition(FilterCondition *cond) {
    printf("  筛选条件:\n");
    if (cond->course_name[0]) {
        printf("    课程名称: %s (%s)\n", cond->course_name,
               cond->fuzzy_match ? "模糊" : "精确");
    }
    if (cond->semester[0]) printf("    学期: %s\n", cond->semester);
    if (cond->score_min != -1 || cond->score_max != -1) {
        printf("    成绩区间: ");
        if (cond->score_min != -1) printf("%d", cond->score_min);
        printf(" - ");
        if (cond->score_max != -1) printf("%d", cond->score_max);
        printf("\n");
    }
    if (cond->college[0]) printf("    学院: %s\n", cond->college);
}
