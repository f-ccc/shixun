/* ============================================================
 * main.c - Main Program Entry Point
 *
 * Course Registration & Big Data Analysis System.
 * CLI menu with 13 functions.
 * All insert inputs fully validated.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "record.h"
#include "list.h"
#include "avl.h"
#include "hash.h"
#include "fileio.h"
#include "operations.h"
#include "filter_sort.h"
#include "statistics.h"
#include "benchmark.h"
#include "generator.h"

#define MAX_RECORDS 100000
#define DATA_FILE "records.csv"

static DataManager *g_dm = NULL;
static StudentRecord g_all[MAX_RECORDS];
static int g_count = 0;
static DataStructureType g_type = DS_HASH;

/*============================================================
 * Input Validation Functions
 *============================================================*/

/* Check if string is all digits */
static int is_all_digits(const char *s) {
    if (!s || !*s) return 0;
    while (*s) { if (!isdigit((unsigned char)*s)) return 0; s++; }
    return 1;
}

/* Check if string is non-empty and not longer than maxlen */
static int is_valid_string(const char *s, int maxlen) {
    if (!s || !*s) return 0;
    return (int)strlen(s) <= maxlen;
}

/* Validate student ID: exactly 12 digits */
static int validate_student_id(const char *sid, int check_exist) {
    StudentRecord tmp;
    if (!sid || strlen(sid) != 12) {
        printf("  Error: Student ID must be exactly 12 digits.\n");
        return 0;
    }
    if (!is_all_digits(sid)) {
        printf("  Error: Student ID must contain only digits.\n");
        return 0;
    }
    if (check_exist) {
        if (dm_find(g_dm, sid, &tmp) == RES_OK) {
            printf("  Error: Student ID '%s' already exists.\n", sid);
            return 0;
        }
    }
    return 1;
}

/* Validate course ID: exactly 8 chars, format XXNNNNNN */
static int validate_course_id(const char *cid) {
    int i;
    if (!cid || strlen(cid) != 8) {
        printf("  Error: Course ID must be exactly 8 characters.\n");
        return 0;
    }
    for (i = 0; i < 2; i++) if (!isalpha((unsigned char)cid[i])) {
        printf("  Error: Course ID first 2 chars must be letters.\n");
        return 0;
    }
    for (i = 2; i < 8; i++) if (!isdigit((unsigned char)cid[i])) {
        printf("  Error: Course ID last 6 chars must be digits.\n");
        return 0;
    }
    return 1;
}

/* Validate name: non-empty, max 19 chars */
static int validate_name(const char *name) {
    if (!is_valid_string(name, 19)) {
        printf("  Error: Name must be non-empty and max 19 characters.\n");
        return 0;
    }
    return 1;
}

/* Validate college: non-empty, max 29 chars */
static int validate_college(const char *col) {
    if (!is_valid_string(col, 29)) {
        printf("  Error: College must be non-empty and max 29 characters.\n");
        return 0;
    }
    return 1;
}

/* Validate course name: non-empty, max 49 chars */
static int validate_course_name(const char *cn) {
    if (!is_valid_string(cn, 49)) {
        printf("  Error: Course name must be non-empty and max 49 characters.\n");
        return 0;
    }
    return 1;
}

/* Validate credit: 0.0 ~ 10.0, step 0.5 */
static int validate_credit(float credit) {
    int steps;
    if (credit < 0.0f || credit > 10.0f) {
        printf("  Error: Credit must be between 0.0 and 10.0.\n");
        return 0;
    }
    steps = (int)(credit * 10.0f + 0.5f);
    if (steps % 5 != 0) {
        printf("  Error: Credit must be in steps of 0.5.\n");
        return 0;
    }
    return 1;
}

/* Validate semester: format YYYY-0N */
static int validate_semester(const char *sem) {
    int y, t;
    if (!sem || strlen(sem) != 7) {
        printf("  Error: Semester format must be YYYY-NN (e.g. 2024-02).\n");
        return 0;
    }
    if (sem[4] != '-') {
        printf("  Error: Semester must contain a dash (YYYY-NN).\n");
        return 0;
    }
    if (sscanf(sem, "%d-%d", &y, &t) != 2 || y < 2020 || y > 2026 || (t != 1 && t != 2)) {
        printf("  Error: Semester year 2020-2026, term 01 or 02.\n");
        return 0;
    }
    return 1;
}

/* Validate date: format YYYY-MM-DD, basic range */
static int validate_date(const char *date) {
    int y, m, d;
    if (!date || strlen(date) != 10) {
        printf("  Error: Date format must be YYYY-MM-DD.\n");
        return 0;
    }
    if (sscanf(date, "%d-%d-%d", &y, &m, &d) != 3 || y < 2020 || y > 2026 || m < 1 || m > 12 || d < 1 || d > 31) {
        printf("  Error: Invalid date, year 2020-2026, month 1-12, day 1-31.\n");
        return 0;
    }
    return 1;
}

/* Validate score: 0-100 */
static int validate_score(int score) {
    if (score < 0 || score > 100) {
        printf("  Error: Score must be between 0 and 100.\n");
        return 0;
    }
    return 1;
}

/* Check duplicate: same student_id + course_id */
static int check_duplicate(DataManager *dm, const char *sid, const char *cid) {
    int total, i, n;
    StudentRecord *arr;
    total = dm_size(dm);
    if (total <= 0) return 0;
    arr = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    n = dm_to_array(dm, arr, total);
    for (i = 0; i < n; i++) {
        if (!strcmp(arr[i].student_id, sid) && !strcmp(arr[i].course_id, cid)) {
            free(arr);
            printf("  Error: Duplicate record! Student '%s' already enrolled in course '%s'.\n", sid, cid);
            return 1;
        }
    }
    free(arr);
    return 0;
}

/*============================================================
 * Menu & Handlers
 *============================================================*/

static void show_menu(void) {
    const char *names[] = {"List", "AVL Tree", "Hash Table"};
    printf("\n--------------------------------------------------------\n");
    printf("  数据结构: %s | 记录数: %d\n", names[g_type], dm_size(g_dm));
    printf("--------------------------------------------------------\n");
    printf("  [1] Insert    [2] Delete    [3] Update    [4] Find\n");
    printf("  [5] Filter    [6] Sort      [7] Stats     [8] Cleanup\n");
    printf("  [9] Bench     [10] Generate [11] Switch   [12] Show All\n");
    printf("  [0] Save & Exit\n");
    printf("--------------------------------------------------------\n");
}

static void handle_insert(void) {
    StudentRecord rec = {0};
    char sid[20], cid[20];
    int ret;

    printf("\n  == 插入记录 ==\n");
    printf("  学号(12位): ");
    if (scanf("%19s", sid) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');
    if (!validate_student_id(sid, 0)) return;
    strcpy(rec.student_id, sid);

    printf("  课程编号(例: CS300101): ");
    if (scanf("%19s", cid) != 1) return;
    while(getchar()!='\n');
    if (!validate_course_id(cid)) return;
    strcpy(rec.course_id, cid);

    /* Check duplicate */
    if (check_duplicate(g_dm, rec.student_id, rec.course_id)) return;

    printf("  姓名: "); scanf("%19s", rec.name); while(getchar()!='\n');
    if (!validate_name(rec.name)) return;

    printf("  学院: "); scanf("%29s", rec.college); while(getchar()!='\n');
    if (!validate_college(rec.college)) return;

    printf("  课程名: "); scanf("%49s", rec.course_name); while(getchar()!='\n');
    if (!validate_course_name(rec.course_name)) return;

    printf("  学分(0.0-10.0,步长0.5): ");
    if (scanf("%f", &rec.credit) != 1) { while(getchar()!='\n'); printf("  Error: Invalid credit format.\n"); return; }
    while(getchar()!='\n');
    if (!validate_credit(rec.credit)) return;

    printf("  学期(YYYY-NN): "); scanf("%7s", rec.semester); while(getchar()!='\n');
    if (!validate_semester(rec.semester)) return;

    printf("  日期(YYYY-MM-DD): "); scanf("%10s", rec.enroll_date); while(getchar()!='\n');
    if (!validate_date(rec.enroll_date)) return;

    printf("  成绩(0-100): ");
    if (scanf("%d", &rec.score) != 1) { while(getchar()!='\n'); printf("  Error: Invalid score.\n"); return; }
    while(getchar()!='\n');
    if (!validate_score(rec.score)) return;

    ret = dm_insert(g_dm, rec);
    if (ret == RES_OK) {
        if (g_count < MAX_RECORDS) g_all[g_count++] = rec;
        printf("  Insert OK. Total: %d\n", dm_size(g_dm));
    } else {
        printf("  Insert failed (code: %d).\n", ret);
    }
}

static void handle_delete(void) {
    char sid[13], cid[9]; int i, ret;
    printf("\n  == Delete Record ==\n");
    printf("  学号: "); scanf("%12s", sid);
    printf("  课程编号: "); scanf("%8s", cid);
    ret = dm_delete(g_dm, sid, cid);
    if (ret == RES_OK) {
        for (i = 0; i < g_count; i++)
            if (!strcmp(g_all[i].student_id, sid) && !strcmp(g_all[i].course_id, cid)) {
                int j; for (j = i; j < g_count - 1; j++) g_all[j] = g_all[j+1];
                g_count--; break;
            }
        printf("  Deleted OK.\n");
    } else printf("  Not found.\n");
}

static void handle_update(void) {
    char sid[13], cid[9]; StudentRecord nr; int i, ret;
    printf("\n  == Update Record ==\n");
    printf("  学号: "); scanf("%12s", sid);
    printf("  课程编号: "); scanf("%8s", cid);
    printf("  新姓名: "); scanf("%19s", nr.name);
    printf("  新学院: "); scanf("%29s", nr.college);
    printf("  新课程名: "); scanf("%49s", nr.course_name);
    printf("  新学分: "); scanf("%f", &nr.credit);
    printf("  新学期: "); scanf("%7s", nr.semester);
    printf("  新日期: "); scanf("%10s", nr.enroll_date);
    printf("  新成绩: "); scanf("%d", &nr.score);
    ret = dm_update(g_dm, sid, cid, nr);
    if (ret == RES_OK) {
        for (i = 0; i < g_count; i++)
            if (!strcmp(g_all[i].student_id, sid) && !strcmp(g_all[i].course_id, cid))
                { strcpy(g_all[i].name, nr.name); strcpy(g_all[i].college, nr.college);
                  strcpy(g_all[i].course_name, nr.course_name); g_all[i].credit = nr.credit;
                  strcpy(g_all[i].semester, nr.semester); strcpy(g_all[i].enroll_date, nr.enroll_date);
                  g_all[i].score = nr.score; break; }
        printf("  Updated OK.\n");
    } else printf("  Not found.\n");
}

static void handle_find(void) {
    char kw[50]; StudentRecord res;
    printf("\n  == Find ==\n  Keyword: "); scanf("%49s", kw);
    if (dm_find(g_dm, kw, &res) == RES_OK) {
        print_header(); print_record(&res);
        printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    } else printf("  Not found.\n");
}

static void handle_filter(void) {
    FilterCondition c; memset(&c,0,sizeof(c)); c.score_min = -1; c.score_max = -1;
    StudentRecord r[MAX_RECORDS]; int ch, n;
    printf("\n  == Filter ==\n  [1]Course [2]Semester [3]Score [4]College [0]Go: ");
    while (1) { printf("\n  选择: "); scanf("%d",&ch); if (!ch) break;
        switch(ch) {
            case 1: printf("  课程: "); scanf("%49s",c.course_name); printf("  模糊(1/0): "); scanf("%d",&c.fuzzy_match); break;
            case 2: printf("  学期: "); scanf("%7s",c.semester); break;
            case 3: printf("  最低(-1=不限): "); scanf("%d",&c.score_min); printf("  最高(-1=不限): "); scanf("%d",&c.score_max); break;
            case 4: printf("  学院: "); scanf("%29s",c.college); break;
        }
    }
    n = filter_records(g_dm, &c, r, MAX_RECORDS);
    printf("\n  Result: %d record(s)\n", n);
    if (n > 0) { print_records(r, n); printf("  导出(1/0): "); scanf("%d",&ch); if (ch) { char fn[100]; printf("  文件名: "); scanf("%99s",fn); export_to_csv(fn,r,n); printf("  Exported to %s\n",fn); } }
}

static void handle_sort(void) {
    int ch, n; StudentRecord *a;
    printf("\n  == Sort ==\n  [1]Ascending [2]Descending: "); scanf("%d",&ch);
    n = dm_size(g_dm); if (!n) { printf("  No data.\n"); return; }
    a = malloc(n*sizeof(StudentRecord));
    n = dm_to_array(g_dm, a, n);
    { SortCondition k[2] = {{"score",ch==1?ASCENDING:DESCENDING},{"student_id",ASCENDING}}; multi_key_sort(a,n,k,2); }
    print_records(a,n);
    printf("  导出(1/0): "); scanf("%d",&ch); if (ch) { char fn[100]; printf("  文件: "); scanf("%99s",fn); export_to_csv(fn,a,n); }
    free(a);
}

static void handle_statistics(void) {
    int ch; printf("\n  == Stats ==\n  [1]Course [2]Student [3]College [4]Semester [5]Score: "); scanf("%d",&ch);
    switch(ch) {
        case 1: { int c; printf("  容量: "); scanf("%d",&c); stat_course_enrollment(g_dm,c); break; }
        case 2: stat_student_courses(g_dm); break;
        case 3: stat_college_distribution(g_dm); break;
        case 4: stat_semester_distribution(g_dm); break;
        case 5: stat_score_distribution(g_dm); break;
    }
}

static void handle_cleanup(void) {
    int i, td=0; StudentRecord *a;
    printf("\n  == Cleanup ==\n  Deleting records before 2023-09-01.\n");
    a = malloc(g_count*sizeof(StudentRecord));
    dm_to_array(g_dm, a, g_count);
    for (i = 0; i < g_count; i++) if (strcmp(a[i].enroll_date, "2023-09-01") < 0) td++;
    free(a);
    if (!td) { printf("  No records to delete.\n"); return; }
    printf("  %d 条记录将删除，确认(1/0): ", td);
    { int cf; scanf("%d",&cf); if (!cf) { printf("  Cancelled.\n"); return; } }
    { int del=0;
    for (i = g_count-1; i >= 0; i--)
        if (strcmp(g_all[i].enroll_date, "2023-09-01") < 0)
            { dm_delete(g_dm, g_all[i].student_id, g_all[i].course_id);
              int j; for (j=i; j<g_count-1; j++) g_all[j]=g_all[j+1];
              g_count--; del++; }
    printf("  Deleted %d record(s). Total: %d\n", del, dm_size(g_dm)); }
}

static void handle_benchmark(void) {
    int ch; printf("\n  == Benchmark ==\n  [1]100 [2]1000 [3]10000: "); scanf("%d",&ch);
    { int sz[]={100,1000,10000}; if (ch>=1&&ch<=3) run_benchmark(sz[ch-1]); else printf("  Invalid.\n"); }
}

static void handle_generate(void) {
    int n, i, ret;
    printf("\n  == Generate ==\n  Count (1-100000): "); scanf("%d",&n);
    if (n<1||n>100000) { printf("  Invalid range.\n"); return; }
    ret = generate_to_csv(DATA_FILE, n);
    if (ret == RES_OK) {
        dm_destroy(g_dm); g_dm = dm_init(g_type);
        g_count = load_records(DATA_FILE, g_all, MAX_RECORDS);
        for (i = 0; i < g_count; i++) dm_insert(g_dm, g_all[i]);
        printf("  Generated %d records. Total: %d\n", n, g_count);
    } else printf("  Generate failed.\n");
}

static void handle_switch_ds(void) {
    int ch, i; DataStructureType nt;
    printf("\n  == Switch ==\n  [1]List [2]AVL Tree [3]Hash Table: "); scanf("%d",&ch);
    switch(ch) { case 1: nt=DS_LIST; break; case 2: nt=DS_AVL; break; case 3: nt=DS_HASH; break; default: return; }
    dm_destroy(g_dm); g_dm = dm_init(nt); g_type = nt;
    for (i = 0; i < g_count; i++) dm_insert(g_dm, g_all[i]);
    printf("  Switched. Records: %d\n", dm_size(g_dm));
}

static void handle_show_all(void) {
    int n = dm_size(g_dm);
    if (!n) { printf("  No records.\n"); return; }
    { StudentRecord *a = malloc(n*sizeof(StudentRecord));
      n = dm_to_array(g_dm, a, n);
      printf("\n  All %d records:\n", n);
      print_records(a, n);
      free(a); }
}

static void save_and_exit(void) {
    int n = dm_size(g_dm);
    StudentRecord *a = malloc(n*sizeof(StudentRecord));
    dm_to_array(g_dm, a, n);
    save_to_csv(DATA_FILE, a, n);
    free(a); dm_destroy(g_dm);
    printf("\n  已保存。谢谢使用!\n");
}

int main(void) {
    int choice, i;
    printf("\n============================================================\n");
    printf("  校园选课记录管理系统\n");
    printf("  数据结构与算法课程设计\n");
    printf("============================================================\n");
    g_dm = dm_init(g_type);
    if (!g_dm) { printf("  Fatal: init failed.\n"); return 1; }
    g_count = load_records(DATA_FILE, g_all, MAX_RECORDS);
    for (i = 0; i < g_count; i++) dm_insert(g_dm, g_all[i]);
    printf("  \u5df2\u52a0\u8f7d\u8bb0\u5f55: %d\n", g_count);
    printf("============================================================\n");
    do {
        show_menu();
        printf("  请选择: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = 0; }
        while(getchar()!='\n');
        switch (choice) {
            case 1: handle_insert(); break; case 2: handle_delete(); break;
            case 3: handle_update(); break; case 4: handle_find(); break;
            case 5: handle_filter(); break; case 6: handle_sort(); break;
            case 7: handle_statistics(); break; case 8: handle_cleanup(); break;
            case 9: handle_benchmark(); break; case 10: handle_generate(); break;
            case 11: handle_switch_ds(); break; case 12: handle_show_all(); break;
            case 0: save_and_exit(); break;
            default: printf("  Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}
