/*
 * benchmark.c - 性能测试实现
 */
#include "benchmark.h"
#include "list.h"
#include "avl.h"
#include "hash.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 获取高精度时间（毫秒） */
static double get_time_ms(void) {
    clock_t t = clock();
    return (double)t / CLOCKS_PER_SEC * 1000.0;
}

double time_insert(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_insert((DLinkedList*)ds, data[i]); break;
            case DS_AVL:  avl_insert((AVLTree*)ds, data[i]); break;
            case DS_HASH: hash_insert((HashTable*)ds, data[i]); break;
        }
    }
    double end = get_time_ms();
    return end - start;
}

double time_find(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_find_by_id((DLinkedList*)ds, data[i].student_id); break;
            case DS_AVL:  avl_find((AVLTree*)ds, data[i].student_id); break;
            case DS_HASH: hash_find((HashTable*)ds, data[i].student_id); break;
        }
    }
    double end = get_time_ms();
    return end - start;
}

double time_delete(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_delete((DLinkedList*)ds, data[i].student_id, data[i].course_id); break;
            case DS_AVL:  avl_delete((AVLTree*)ds, data[i].student_id, data[i].course_id); break;
            case DS_HASH: hash_delete((HashTable*)ds, data[i].student_id, data[i].course_id); break;
        }
    }
    double end = get_time_ms();
    return end - start;
}

double time_traverse(void *ds, DataStructureType type, int n) {
    /* 空遍历函数 */
    StudentRecord *arr = (StudentRecord*)malloc(n * sizeof(StudentRecord));
    double start = get_time_ms();

    switch (type) {
        case DS_LIST: list_to_array((DLinkedList*)ds, arr, n); break;
        case DS_AVL:  avl_inorder_to_array(((AVLTree*)ds)->root, arr, 0); break;
        case DS_HASH: hash_to_array((HashTable*)ds, arr, n); break;
    }

    double end = get_time_ms();
    free(arr);
    return end - start;
}

double time_sort(void *ds, DataStructureType type, int n) {
    StudentRecord *arr = (StudentRecord*)malloc(n * sizeof(StudentRecord));
    double start = get_time_ms();

    switch (type) {
        case DS_LIST: {
            list_to_array((DLinkedList*)ds, arr, n);
            list_sort_by_score((DLinkedList*)ds, 1);
            break;
        }
        case DS_AVL:
            avl_sort_by_score((AVLTree*)ds, arr, 1);
            break;
        case DS_HASH:
            hash_sort_by_score((HashTable*)ds, arr, 1);
            break;
    }

    double end = get_time_ms();
    free(arr);
    return end - start;
}

size_t estimate_memory(void *ds, DataStructureType type) {
    switch (type) {
        case DS_LIST: {
            DLinkedList *list = (DLinkedList*)ds;
            return sizeof(DLinkedList) + list->size * sizeof(DListNode);
        }
        case DS_AVL: {
            AVLTree *tree = (AVLTree*)ds;
            return sizeof(AVLTree) + tree->size * sizeof(AVLNode);
        }
        case DS_HASH: {
            HashTable *table = (HashTable*)ds;
            return sizeof(HashTable) + table->size * sizeof(HashNode);
        }
    }
    return 0;
}

/* 销毁所有数据结构 */
static void destroy_all(void *ds, DataStructureType type) {
    switch (type) {
        case DS_LIST: list_destroy((DLinkedList*)ds); break;
        case DS_AVL:  avl_destroy((AVLTree*)ds); break;
        case DS_HASH: hash_destroy((HashTable*)ds); break;
    }
}

/* 创建空的数据结构 */
static void* create_empty(DataStructureType type) {
    switch (type) {
        case DS_LIST: return list_init();
        case DS_AVL:  return avl_init();
        case DS_HASH: return hash_init();
    }
    return NULL;
}

void run_benchmark(int data_size) {
    int i, t;
    printf("\n=================================================================\n");
    printf("  性能测试报告 (数据规模: %d 条)\n", data_size);
    printf("=================================================================\n");

    /* 生成测试数据 */
    StudentRecord *data = (StudentRecord*)malloc(data_size * sizeof(StudentRecord));
    generate_records(data, data_size);

    const char *type_names[] = {"双向链表", "AVL树", "哈希表"};
    DataStructureType types[] = {DS_LIST, DS_AVL, DS_HASH};
    int type_count = 3;

    /* 表头 */
    printf("\n+------------+--------------+------------+------------+------------+\n");
    printf("| 操作       | 数据结构     | 耗时(ms)   | 内存(MB)   | 单次(us)   |\n");
    printf("+------------+--------------+------------+------------+------------+\n");

    for (t = 0; t < type_count; t++) {
        void *ds = create_empty(types[t]);

        /* 插入测试 */
        double t_insert = time_insert(ds, types[t], data, data_size);
        double avg_insert = t_insert / data_size * 1000; /* us */

        /* 查找测试 */
        double t_find = time_find(ds, types[t], data, data_size);
        double avg_find = t_find / data_size * 1000;

        /* 遍历测试 */
        double t_trav = time_traverse(ds, types[t], data_size);

        /* 排序测试 */
        double t_sort = time_sort(ds, types[t], data_size);

        /* 内存 */
        size_t mem = estimate_memory(ds, types[t]);
        double mem_mb = (double)mem / (1024.0 * 1024.0);

        /* 输出 */
        printf("| 插入       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               type_names[t], t_insert, mem_mb, avg_insert);
        printf("| 查找       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               type_names[t], t_find, mem_mb, avg_find);
        printf("| 遍历       | %-12s | %10.3f | %10.2f | %10s |\n",
               type_names[t], t_trav, mem_mb, "");
        printf("| 排序       | %-12s | %10.3f | %10.2f | %10s |\n",
               type_names[t], t_sort, mem_mb, "");

        /* 删除测试前重新插入 */
        destroy_all(ds, types[t]);
        ds = create_empty(types[t]);
        for (i = 0; i < data_size; i++) {
            switch (types[t]) {
                case DS_LIST: list_insert((DLinkedList*)ds, data[i]); break;
                case DS_AVL:  avl_insert((AVLTree*)ds, data[i]); break;
                case DS_HASH: hash_insert((HashTable*)ds, data[i]); break;
            }
        }
        double t_delete = time_delete(ds, types[t], data, data_size);
        double avg_delete = t_delete / data_size * 1000;
        printf("| 删除       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               type_names[t], t_delete, mem_mb, avg_delete);
        printf("+------------+--------------+------------+------------+------------+\n");

        destroy_all(ds, types[t]);
    }

    /* 复杂度分析 */
    printf("\n  理论时间复杂度对比:\n");
    printf("  +------------+------------+------------+------------+\n");
    printf("  | 操作       | 双向链表   | AVL树      | 哈希表     |\n");
    printf("  +------------+------------+------------+------------+\n");
    printf("  | 插入       | O(1)       | O(log n)   | O(1)平均   |\n");
    printf("  | 查找       | O(n)       | O(log n)   | O(1)平均   |\n");
    printf("  | 删除       | O(n)       | O(log n)   | O(1)平均   |\n");
    printf("  | 遍历       | O(n)       | O(n)       | O(n)       |\n");
    printf("  | 排序       | O(nlog n)  | O(n)中序   | O(nlog n)  |\n");
    printf("  +------------+------------+------------+------------+\n");

    free(data);
    printf("\n  性能测试完成。\n");
}
