/*
 * main.c - 主程序入口
 * 校园选课记录检索与大数据分析系统
 * 数据结构与算法课程设计
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

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

/* 全局数据管理器 */
static DataManager *g_dm = NULL;

/* 存储所有记录（用于多结构同步） */
static StudentRecord g_all_records[MAX_RECORDS];
static int g_record_count = 0;

/* 当前使用的数据结构类型 */
static DataStructureType g_current_type = DS_HASH;

/* 函数声明 */
static void show_menu(void);
static void handle_insert(void);
static void handle_delete(void);
static void handle_update(void);
static void handle_find(void);
static void handle_filter(void);
static void handle_sort(void);
static void handle_statistics(void);
static void handle_cleanup(void);
static void handle_benchmark(void);
static void handle_generate(void);
static void handle_switch_ds(void);
static void handle_show_all(void);
static void save_and_exit(void);

int main(void) {
#ifdef _WIN32
    /* 设置控制台为UTF-8编码，解决中文乱码 */
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    /* 初始化 */
    printf("\n");
    printf("============================================================\n");
    printf("   校园选课记录检索与大数据分析系统\n");
    printf("   数据结构与算法课程设计\n");
    printf("============================================================\n");

    /* 初始化数据管理器（默认使用哈希表） */
    g_dm = dm_init(g_current_type);
    if (!g_dm) {
        printf("  错误: 初始化数据管理器失败!\n");
        return 1;
    }

    /* 从文件加载数据 */
    g_record_count = load_records(DATA_FILE, g_all_records, MAX_RECORDS);
    for (int i = 0; i < g_record_count; i++) {
        dm_insert(g_dm, g_all_records[i]);
    }
    printf("  当前记录数: %d\n", g_record_count);
    printf("============================================================\n");

    int choice;
    do {
        show_menu();
        printf("  请输入选项: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }
        while (getchar() != '\n'); /* 清空缓冲区 */

        switch (choice) {
            case 1: handle_insert(); break;
            case 2: handle_delete(); break;
            case 3: handle_update(); break;
            case 4: handle_find(); break;
            case 5: handle_filter(); break;
            case 6: handle_sort(); break;
            case 7: handle_statistics(); break;
            case 8: handle_cleanup(); break;
            case 9: handle_benchmark(); break;
            case 10: handle_generate(); break;
            case 11: handle_switch_ds(); break;
            case 12: handle_show_all(); break;
            case 0: save_and_exit(); break;
            default: printf("  无效选项，请重新输入。\n"); break;
        }
    } while (choice != 0);

    return 0;
}

static void show_menu(void) {
    const char *type_names[] = {"双向链表", "AVL树", "哈希表"};
    printf("\n-----------------------------------------------------------------\n");
    printf("  当前数据结构: %s | 记录数: %d\n",
           type_names[g_current_type], dm_size(g_dm));
    printf("-----------------------------------------------------------------\n");
    printf("  [1] 插入记录       [2] 删除记录       [3] 修改记录\n");
    printf("  [4] 查找记录       [5] 筛选记录       [6] 多关键字排序\n");
    printf("  [7] 统计分析       [8] 过期记录清理   [9] 性能测试\n");
    printf("  [10] 生成数据      [11] 切换数据结构  [12] 显示全部\n");
    printf("  [0] 保存并退出\n");
    printf("-----------------------------------------------------------------\n");
}

static void handle_insert(void) {
    StudentRecord rec;
    printf("\n  == 插入选课记录 ==\n");
    printf("  学号(12位): "); scanf("%12s", rec.student_id);
    printf("  姓名: "); scanf("%19s", rec.name);
    printf("  学院: "); scanf("%29s", rec.college);
    printf("  课程编号(8位): "); scanf("%8s", rec.course_id);
    printf("  课程名称: "); scanf("%49s", rec.course_name);
    printf("  学分: "); scanf("%f", &rec.credit);
    printf("  选课学期(如2024-02): "); scanf("%6s", rec.semester);
    printf("  选课日期(YYYY-MM-DD): "); scanf("%10s", rec.enroll_date);
    printf("  成绩(0-100): "); scanf("%d", &rec.score);

    /* 简单校验 */
    if (rec.score < 0 || rec.score > 100) {
        printf("  错误: 成绩必须在0-100之间!\n");
        return;
    }

    int ret = dm_insert(g_dm, rec);
    if (ret == RES_OK) {
        if (g_record_count < MAX_RECORDS)
            g_all_records[g_record_count++] = rec;
        printf("  插入成功! 当前记录数: %d\n", dm_size(g_dm));
    } else if (ret == RES_DUP) {
        printf("  错误: 该学号已存在!\n");
    } else {
        printf("  插入失败!\n");
    }
}

static void handle_delete(void) {
    char sid[13], cid[9];
    printf("\n  == 删除选课记录 ==\n");
    printf("  学号: "); scanf("%12s", sid);
    printf("  课程编号: "); scanf("%8s", cid);

    int ret = dm_delete(g_dm, sid, cid);
    if (ret == RES_OK) {
        /* 也从全局数组删除 */
        for (int i = 0; i < g_record_count; i++) {
            if (strcmp(g_all_records[i].student_id, sid) == 0 &&
                strcmp(g_all_records[i].course_id, cid) == 0) {
                for (int j = i; j < g_record_count - 1; j++)
                    g_all_records[j] = g_all_records[j + 1];
                g_record_count--;
                break;
            }
        }
        printf("  删除成功!\n");
    } else {
        printf("  未找到该记录。\n");
    }
}

static void handle_update(void) {
    char sid[13], cid[9];
    StudentRecord new_rec;

    printf("\n  == 修改选课记录 ==\n");
    printf("  要修改的学号: "); scanf("%12s", sid);
    printf("  要修改的课程编号: "); scanf("%8s", cid);

    /* 输入新数据 */
    printf("  新姓名: "); scanf("%19s", new_rec.name);
    printf("  新学院: "); scanf("%29s", new_rec.college);
    printf("  新课程名称: "); scanf("%49s", new_rec.course_name);
    printf("  新学分: "); scanf("%f", &new_rec.credit);
    printf("  新学期(如2024-02): "); scanf("%6s", new_rec.semester);
    printf("  新日期(YYYY-MM-DD): "); scanf("%10s", new_rec.enroll_date);
    printf("  新成绩(0-100): "); scanf("%d", &new_rec.score);

    int ret = dm_update(g_dm, sid, cid, new_rec);
    if (ret == RES_OK) {
        /* 更新全局数组 */
        for (int i = 0; i < g_record_count; i++) {
            if (strcmp(g_all_records[i].student_id, sid) == 0 &&
                strcmp(g_all_records[i].course_id, cid) == 0) {
                strcpy(g_all_records[i].name, new_rec.name);
                strcpy(g_all_records[i].college, new_rec.college);
                strcpy(g_all_records[i].course_name, new_rec.course_name);
                g_all_records[i].credit = new_rec.credit;
                strcpy(g_all_records[i].semester, new_rec.semester);
                strcpy(g_all_records[i].enroll_date, new_rec.enroll_date);
                g_all_records[i].score = new_rec.score;
                break;
            }
        }
        printf("  修改成功!\n");
    } else {
        printf("  未找到该记录。\n");
    }
}

static void handle_find(void) {
    char keyword[50];
    printf("\n  == 查找记录 ==\n");
    printf("  请输入学号(精确)或姓名(精确): ");
    scanf("%49s", keyword);

    /* 先按学号查找 */
    StudentRecord result;
    if (dm_find(g_dm, keyword, &result) == RES_OK) {
        printf("  找到记录:\n");
        print_header();
        print_record(&result);
        printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
        return;
    }

    /* 再按姓名查找 */
    printf("  未按学号找到，尝试按姓名查找...\n");
    /* 遍历所有记录找姓名匹配 */
    int total = dm_size(g_dm);
    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(g_dm, all, total);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(all[i].name, keyword) == 0) {
            if (!found) {
                printf("  找到以下记录:\n");
                print_header();
                found = 1;
            }
            print_record(&all[i]);
        }
    }
    if (found) {
        printf("+--------------+----------+------------------+----------+----------------------+-----+---------+------------+-----+\n");
    } else {
        printf("  未找到匹配的记录。\n");
    }
    free(all);
}

static void handle_filter(void) {
    FilterCondition cond;
    memset(&cond, 0, sizeof(cond));
    cond.score_min = -1;
    cond.score_max = -1;

    int choice;
    printf("\n  == 多条件筛选 ==\n");
    printf("  请选择筛选条件(可多选):\n");
    printf("  [1] 按课程名称\n");
    printf("  [2] 按选课学期\n");
    printf("  [3] 按成绩区间\n");
    printf("  [4] 按学院\n");
    printf("  [0] 开始筛选\n");

    while (1) {
        printf("  选择条件: ");
        scanf("%d", &choice);
        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("  课程名称: "); scanf("%49s", cond.course_name);
                printf("  模糊匹配? (1=是, 0=精确): ");
                scanf("%d", &cond.fuzzy_match);
                break;
            case 2:
                printf("  学期(如2024-02): "); scanf("%6s", cond.semester);
                break;
            case 3:
                printf("  最低成绩(-1=不限制): "); scanf("%d", &cond.score_min);
                printf("  最高成绩(-1=不限制): "); scanf("%d", &cond.score_max);
                break;
            case 4:
                printf("  学院: "); scanf("%29s", cond.college);
                break;
            default:
                printf("  无效选项。\n");
                break;
        }
    }

    StudentRecord results[MAX_RECORDS];
    int n = filter_records(g_dm, &cond, results, MAX_RECORDS);

    printf("\n  筛选结果: 共 %d 条记录\n", n);
    if (n > 0) {
        print_records(results, n);

        /* 询问是否导出 */
        printf("  是否导出到文件? (1=是, 0=否): ");
        int export_choice;
        scanf("%d", &export_choice);
        if (export_choice) {
            char fname[100];
            printf("  文件名: "); scanf("%99s", fname);
            if (export_to_csv(fname, results, n) == RES_OK)
                printf("  已导出到 %s\n", fname);
            else
                printf("  导出失败!\n");
        }
    }
}

static void handle_sort(void) {
    printf("\n  == 多关键字排序 ==\n");
    printf("  按成绩排序:\n");
    printf("  [1] 升序 (低->高)\n");
    printf("  [2] 降序 (高->低)\n");
    printf("  请选择: ");

    int choice;
    scanf("%d", &choice);

    int total = dm_size(g_dm);
    if (total <= 0) { printf("  暂无数据。\n"); return; }

    StudentRecord *arr = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(g_dm, arr, total);

    if (choice == 1) {
        /* 先按成绩升序，相同学号升序 */
        SortCondition keys[2] = {
            {"score", ASCENDING},
            {"student_id", ASCENDING}
        };
        multi_key_sort(arr, n, keys, 2);
    } else {
        SortCondition keys[2] = {
            {"score", DESCENDING},
            {"student_id", ASCENDING}
        };
        multi_key_sort(arr, n, keys, 2);
    }

    print_records(arr, n);

    /* 询问导出 */
    printf("  是否导出? (1=是, 0=否): ");
    scanf("%d", &choice);
    if (choice) {
        char fname[100];
        printf("  文件名: "); scanf("%99s", fname);
        export_to_csv(fname, arr, n);
        printf("  已导出。\n");
    }
    free(arr);
}

static void handle_statistics(void) {
    printf("\n  == 数据统计分析 ==\n");
    printf("  [1] 每门课程选课人数与容量使用率\n");
    printf("  [2] 每位学生选课门数与总学分\n");
    printf("  [3] 各学院选课人数分布\n");
    printf("  [4] 按学期统计选课人数与课程数分布\n");
    printf("  [5] 课程成绩分布统计\n");
    printf("  请选择: ");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int cap;
            printf("  请输入课程容量: ");
            scanf("%d", &cap);
            stat_course_enrollment(g_dm, cap);
            break;
        }
        case 2: stat_student_courses(g_dm); break;
        case 3: stat_college_distribution(g_dm); break;
        case 4: stat_semester_distribution(g_dm); break;
        case 5: stat_score_distribution(g_dm); break;
        default: printf("  无效选项。\n"); break;
    }
}

static void handle_cleanup(void) {
    printf("\n  == 过期记录清理 ==\n");
    printf("  基准日期: 2026-09-01\n");
    printf("  将删除选课日期早于 2023-09-01 (3年前) 的记录。\n");

    /* 统计需要删除的记录 */
    int total = dm_size(g_dm);
    StudentRecord *all = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(g_dm, all, total);

    int to_delete = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(all[i].enroll_date, "2023-09-01") < 0) {
            to_delete++;
        }
    }
    free(all);

    if (to_delete == 0) {
        printf("  无需删除的过期记录。\n");
        return;
    }

    printf("  即将删除 %d 条过期记录。\n", to_delete);
    printf("  确认删除? (1=是, 0=否): ");
    int confirm;
    scanf("%d", &confirm);

    if (!confirm) {
        printf("  取消删除。\n");
        return;
    }

    /* 执行删除 */
    int deleted = 0;
    for (int i = g_record_count - 1; i >= 0; i--) {
        if (strcmp(g_all_records[i].enroll_date, "2023-09-01") < 0) {
            /* 从所有结构中删除 */
            dm_delete(g_dm, g_all_records[i].student_id, g_all_records[i].course_id);
            /* 从全局数组删除 */
            for (int j = i; j < g_record_count - 1; j++)
                g_all_records[j] = g_all_records[j + 1];
            g_record_count--;
            deleted++;
        }
    }

    /* 重新加载到当前管理器 */
    /* 已通过dm_delete同步删除 */
    printf("  删除完成! 共清理 %d 条过期记录。当前记录数: %d\n", deleted, dm_size(g_dm));
}

static void handle_benchmark(void) {
    printf("\n  == 性能测试 ==\n");
    printf("  请选择数据规模:\n");
    printf("  [1] 100条\n");
    printf("  [2] 1000条\n");
    printf("  [3] 10000条\n");
    printf("  请选择: ");

    int choice;
    scanf("%d", &choice);

    int sizes[] = {100, 1000, 10000};
    if (choice >= 1 && choice <= 3) {
        run_benchmark(sizes[choice - 1]);
    } else {
        printf("  无效选项。\n");
    }
}

static void handle_generate(void) {
    printf("\n  == 生成测试数据 ==\n");
    printf("  请输入要生成的记录数: ");
    int count;
    scanf("%d", &count);

    if (count <= 0 || count > 100000) {
        printf("  数量应在 1-100000 之间。\n");
        return;
    }

    printf("  正在生成 %d 条数据...\n", count);
    int ret = generate_to_csv(DATA_FILE, count);
    if (ret == RES_OK) {
        printf("  数据已生成并保存到 %s\n", DATA_FILE);
        /* 重新加载 */
        dm_destroy(g_dm);
        g_dm = dm_init(g_current_type);
        g_record_count = load_records(DATA_FILE, g_all_records, MAX_RECORDS);
        for (int i = 0; i < g_record_count; i++)
            dm_insert(g_dm, g_all_records[i]);
        printf("  已重新加载数据。当前记录数: %d\n", g_record_count);
    } else {
        printf("  生成失败!\n");
    }
}

static void handle_switch_ds(void) {
    printf("\n  == 切换数据结构 ==\n");
    printf("  [1] 双向链表\n");
    printf("  [2] AVL树\n");
    printf("  [3] 哈希表\n");
    printf("  请选择: ");

    int choice;
    scanf("%d", &choice);

    DataStructureType new_type;
    switch (choice) {
        case 1: new_type = DS_LIST; break;
        case 2: new_type = DS_AVL; break;
        case 3: new_type = DS_HASH; break;
        default: printf("  无效选项。\n"); return;
    }

    /* 创建新结构并复制数据 */
    dm_destroy(g_dm);
    g_dm = dm_init(new_type);
    g_current_type = new_type;
    for (int i = 0; i < g_record_count; i++)
        dm_insert(g_dm, g_all_records[i]);
    printf("  已切换到 %s。当前记录数: %d\n",
           new_type == DS_LIST ? "双向链表" :
           new_type == DS_AVL ? "AVL树" : "哈希表",
           dm_size(g_dm));
}

static void handle_show_all(void) {
    int total = dm_size(g_dm);
    if (total <= 0) {
        printf("  暂无记录。\n");
        return;
    }

    StudentRecord *arr = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(g_dm, arr, total);

    printf("\n  全部记录 (%d 条):\n", n);
    print_records(arr, n);
    free(arr);
}

static void save_and_exit(void) {
    printf("\n  正在保存数据到 %s ...\n", DATA_FILE);
    int total = dm_size(g_dm);
    StudentRecord *arr = (StudentRecord*)malloc(total * sizeof(StudentRecord));
    int n = dm_to_array(g_dm, arr, total);
    save_to_csv(DATA_FILE, arr, n);
    free(arr);

    dm_destroy(g_dm);
    printf("  数据已保存。感谢使用!\n");
    printf("============================================================\n");
}
