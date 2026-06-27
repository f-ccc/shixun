/* ============================================================
 * filter_sort.c - Filter & Sort Implementation
 *
 * filter_records: linear scan with condition checks.
 * multi_key_sort: nested bubble sort over key list.
 * print_record/header/records: ASCII table formatting.
 * ============================================================ */

#include "filter_sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int filter_records(DataManager *dm, FilterCondition *cond, StudentRecord *result, int max_result) {
    int i, total, n, count = 0, match;
    StudentRecord *all;
    if (!dm) return 0;
    total = dm_size(dm);
    if (total <= 0) return 0;
    all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, all, total);
    for (i = 0; i < n && count < max_result; i++) {
        match = 1;
        if (cond->course_name[0]) {
            if (cond->fuzzy_match) { if (!strstr(all[i].course_name, cond->course_name)) match = 0; }
            else { if (strcmp(all[i].course_name, cond->course_name)) match = 0; }
        }
        if (match && cond->semester[0] && strcmp(all[i].semester, cond->semester)) match = 0;
        if (match && cond->score_min != -1 && all[i].score < cond->score_min) match = 0;
        if (match && cond->score_max != -1 && all[i].score > cond->score_max) match = 0;
        if (match && cond->college[0] && strcmp(all[i].college, cond->college)) match = 0;
        if (match) result[count++] = all[i];
    }
    free(all);
    return count;
}

int multi_key_sort(StudentRecord *arr, int size, SortCondition *sort_keys, int key_count) {
    int i, j, k, cmp, need_swap;
    StudentRecord tmp;
    if (size <= 1 || key_count <= 0) return size;
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - 1 - i; j++) {
            need_swap = 0;
            for (k = 0; k < key_count; k++) {
                cmp = 0;
                if (!strcmp(sort_keys[k].field, "score")) cmp = arr[j].score - arr[j+1].score;
                else if (!strcmp(sort_keys[k].field, "student_id")) cmp = strcmp(arr[j].student_id, arr[j+1].student_id);
                else if (!strcmp(sort_keys[k].field, "name")) cmp = strcmp(arr[j].name, arr[j+1].name);
                if (cmp) { need_swap = sort_keys[k].dir ? (cmp > 0) : (cmp < 0); break; }
            }
            if (need_swap) { tmp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = tmp; }
        }
    return size;
}

void print_record(const StudentRecord *rec) {
    printf("| %-12s | %-8s | %-16s | %-8s | %-20s | %.1f | %-7s | %-10s | %3d |\n",
           rec->student_id, rec->name, rec->college, rec->course_id, rec->course_name,
           rec->credit, rec->semester, rec->enroll_date, rec->score);
}

void print_header(void) {
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    printf("| 学号         | 姓名     | 学院             | 课程编号 | 课程名称             | 学分 | 学期    | 选课日期   | 成绩 |\n");
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
}

void print_records(const StudentRecord *arr, int size) {
    int i;
    if (size <= 0) { printf("  (无匹配记录)\n"); return; }
    print_header();
    for (i = 0; i < size; i++) print_record(&arr[i]);
    printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    printf("  总计: %d 条记录\n", size);
}
