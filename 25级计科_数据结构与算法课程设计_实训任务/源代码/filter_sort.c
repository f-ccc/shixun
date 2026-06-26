/*
 * filter_sort.c - 多条件筛选与多关键字排序实现
 */
#include "filter_sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int filter_records(DataManager *dm, FilterCondition *cond,
                   StudentRecord *result, int max_result) {
    int i, total, n, count, match;
    StudentRecord *all;
    if (!dm) return 0;
    total = dm_size(dm);
    if (total <= 0) return 0;
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    count = 0;
    for (i = 0; i < n && count < max_result; i++) {
        match = 1;
        if (cond->course_name[0] != '\0') {
            if (cond->fuzzy_match) {
                if (strstr(all[i].course_name, cond->course_name) == NULL)
                    match = 0;
            } else {
                if (strcmp(all[i].course_name, cond->course_name) != 0)
                    match = 0;
            }
        }
        if (match && cond->semester[0] != '\0') {
            if (strcmp(all[i].semester, cond->semester) != 0) match = 0;
        }
        if (match && cond->score_min != -1) {
            if (all[i].score < cond->score_min) match = 0;
        }
        if (match && cond->score_max != -1) {
            if (all[i].score > cond->score_max) match = 0;
        }
        if (match && cond->college[0] != '\0') {
            if (strcmp(all[i].college, cond->college) != 0) match = 0;
        }
        if (match) result[count++] = all[i];
    }
    free(all);
    return count;
}

int multi_key_sort(StudentRecord *arr, int size,
                   SortCondition *sort_keys, int key_count) {
    int i, j, k, cmp, need_swap;
    StudentRecord tmp;
    if (size <= 1 || key_count <= 0) return size;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - 1 - i; j++) {
            need_swap = 0;
            for (k = 0; k < key_count; k++) {
                cmp = 0;
                if (strcmp(sort_keys[k].field, "score") == 0 ||
                    strcmp(sort_keys[k].field, "\u6210\u7ee9") == 0) {
                    cmp = arr[j].score - arr[j+1].score;
                } else if (strcmp(sort_keys[k].field, "student_id") == 0 ||
                           strcmp(sort_keys[k].field, "\u5b66\u53f7") == 0) {
                    cmp = strcmp(arr[j].student_id, arr[j+1].student_id);
                } else if (strcmp(sort_keys[k].field, "name") == 0 ||
                           strcmp(sort_keys[k].field, "\u59d3\u540d") == 0) {
                    cmp = strcmp(arr[j].name, arr[j+1].name);
                }
                if (cmp != 0) {
                    need_swap = sort_keys[k].dir ? (cmp > 0) : (cmp < 0);
                    break;
                }
            }
            if (need_swap) {
                tmp = arr[j];
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
    printf("| \u5b66\u53f7         | \u59d3\u540d    | \u5b66\u9662             | \u8bfe\u7a0b\u7f16\u53f7 | \u8bfe\u7a0b\u540d\u79f0             | \u5b66\u5206 | \u5b66\u671f   | \u9009\u8bfe\u65e5\u671f   | \u6210\u7ee9 |\n");
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
}

void print_records(const StudentRecord *arr, int size) {
    int i;
    if (size <= 0) {
        printf("  (\u65e0\u8bb0\u5f55)\n");
        return;
    }
    print_header();
    for (i = 0; i < size; i++) {
        print_record(&arr[i]);
    }
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    printf("  \u5171 %d \u6761\u8bb0\u5f55\n", size);
}

void print_filter_condition(FilterCondition *cond) {
    printf("  \u7b5b\u9009\u6761\u4ef6:\n");
    if (cond->course_name[0]) {
        printf("    \u8bfe\u7a0b\u540d\u79f0: %s (%s)\n", cond->course_name,
               cond->fuzzy_match ? "\u6a21\u7cca" : "\u7cbe\u786e");
    }
    if (cond->semester[0]) printf("    \u5b66\u671f: %s\n", cond->semester);
    if (cond->score_min != -1 || cond->score_max != -1) {
        printf("    \u6210\u7ee9\u533a\u95f4: ");
        if (cond->score_min != -1) printf("%d", cond->score_min);
        printf(" - ");
        if (cond->score_max != -1) printf("%d", cond->score_max);
        printf("\n");
    }
    if (cond->college[0]) printf("    \u5b66\u9662: %s\n", cond->college);
}
